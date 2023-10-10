#include "tableGenerator.h"

enum {
	TERMIANL_START = 1,
	NUM,      //number
	ADD,      // +
	SUB,      // -
	MUL,      // *
	DIV,      // /
	L_PARENTHESES, // (
	R_PARENTHESES, // )

	PLUS, //+
	MINUS, //-
	ASTARISK, //*
	SLASH, // /

	FIRST,

	NON_TERMINAL_START = 256,
	EXPR, // E
	TERM, // T
	FACTOR, // F
	ACCEPTED, //accept

	END, //$ temporary

	S,  // shift
	R, // reducce
	G, //goto
	ACC, //acc
};

//pre defined Productions
//{EXPR, ADD, TERM}
//   0    1     2
Production productions[] = {
	{0, EXPR, {EXPR, END, -1, -1, -1, -1, -1, -1, -1, -1}, -1, -1},
	{1, EXPR, {EXPR, ADD, TERM, -1, -1, -1, -1, -1, -1, -1}, -1, -1},
	{2, EXPR, {EXPR, SUB, TERM, -1, -1, -1, -1, -1, -1, -1}, -1, -1},
	{3, EXPR, {TERM, -1, -1, -1, -1, -1, -1, -1, -1, -1}, -1, -1},
	{4, TERM, {TERM, MUL, FACTOR, -1, -1, -1, -1, -1, -1, -1}, -1, -1},
	{5, TERM, {TERM, DIV, FACTOR, -1, -1, -1, -1, -1, -1, -1}, -1, -1},
	{6, TERM, {FACTOR, -1, -1, -1, -1, -1, -1, -1, -1, -1}, -1, -1},
	{7, FACTOR, {L_PARENTHESES, EXPR, R_PARENTHESES, -1, -1, -1, -1, -1, -1, -1}, -1, -1},
	{8, FACTOR, {NUM, -1, -1, -1, -1, -1, -1, -1, -1, -1}, -1, -1},
};


//may not needed
void appendCopiedOriginalProd(DynamicArray *arr, int key, Type type) {
	if (type != PRODUCTION) error("type mismatch: appendCopiedOriginalProd");
	appendCopy(arr, &productions[key], type);
}

DynamicArray *insertProdsFromPreDefined() {
	int size = ARRAY_LENGTH(productions);
	DynamicArray *originalProdArray = createDynamicArray(size, true, dummy_member, PRODUCTION);

	for (int i = 0; i < size; i++) {
		appendCopiedOriginalProd(originalProdArray, i, PRODUCTION);
	}
	return originalProdArray;
}

DynamicArray *setUpPreDefinedProd() {
	DynamicArray *originalProdArary = insertProdsFromPreDefined();
	quickSort(originalProdArary, getCur_symbol, 0, getArrayOffset(originalProdArary), PRODUCTION);
	return originalProdArary;
}

void printProd(DynamicArray *arr, Type type) {
	if (type != PRODUCTION) error("type mismatch: printProd\n");

	for (int i = 0; i < getArraySize(arr); i++) {
		Production *prod = (Production *)retriveData(arr, i, PRODUCTION);
		printf("prod key: %d %p\n", prod->key, &(prod->key));
	}
}

bool isNonTerminal(int symbol) {
	return symbol >= NON_TERMIANL_START;
}

int getLeftFromProd(Data *data, Type type) {
	if (type != PRODUCTION) error("type mismatch: getLeftFromProd\n");
	return ((Production*)data)->left;
}

int getCur_symbol(Data *data, Type type) {
	if (type != PRODUCTION) error("type mismatch: getCur_symbol\n");
	Production *prod = (Production *)(data);
	return prod->cur_symbol;
}

int getCur_symbolForOverlapHandler(Data *data, Type type) {
	if (type != PRODUCTION) error("type mismatch: getCur_symbol\n");
	Production *prod = (Production *)(data);
	return prod->cur_symbol - NON_TERMINAL_START;
}

bool cmpCur_symbol(Data *data, Data *expectedData, Type type) {
	if (type != PRODUCTION) error("type mismatch: cmpCur_symbol\n");
	Production *symbol = (Production *)data;
	Production *compedValue = (Production *)expectedData;
	return symbol->cur_symbol == compedValue->cur_symbol;
}

bool cmpHash_keyOfProdsInItem(Data *data, Data *expectedData, Type type) {
	if (type != ITEM) error("type mismatch: cmpHash_keyOfProdsInItem\n");
	Item *item = (Item *)data;
	Item *cmpedItem = (Item *)expectedData;
	return item->hashed_keys == cmpedItem->hashed_keys;
}

int getHash_keyOfProdsInItem(Data *data, Data *expectedData, Type type) {
	if (type != ITEM) error("type mismatch: getHash_keyOfProdsInItem\n");
	Item *item = (Item *)data;
	return item->hashed_keys;
}

bool cmpLeftFromProd(Data *data, Data *expectedValue, Type type) {
	if (type != PRODUCTION) error("type mismatch: cmpLefFromProd\n");
	Production *symbol = (Production *)data;
	Production *compedValue = (Production *)expectedValue;
	return symbol->left == compedValue->left;
}

void updateCur_Symbol(DynamicArray *prodArr, Type type) {
	if (type != PRODUCTION) error("type mismatch: setSymbol\n");
	for (int i = 0; i < getArraySize(prodArr); i++) {
		Production *prod = (Production *)retriveData(prodArr, i, PRODUCTION);
		//look ahead and increament
		//if (prod->cur_symbol == -1) continue;
		int pos = ++prod->readPosition;
		//update cur_symbol
		int symbol = prod->right[pos];
		prod->cur_symbol = symbol;
	}
}

DynamicArray *Clone_UpdateProds(DynamicArray *oldProdArray, Type type) {
	if (type != PRODUCTION) error("type mismatch: setSymbol\n");
	DynamicArray *clonedProdsArray = cloneArray(oldProdArray, true, oldProdArray->referentMember);
	quickSort(clonedProdsArray, getLeftFromProd, 0, getArrayOffset(clonedProdsArray), PRODUCTION);
	updateCur_Symbol(clonedProdsArray, type);
	return clonedProdsArray;
}

//make this optimize later particulary using deprioritizer
bool isEndProd(DynamicArray *prodArr) {
	if (getArraySize(prodArr) != 1) return false;
	Production *singleProd = (Production *)retriveData(prodArr, 0, PRODUCTION);
	int pos_read = singleProd->readPosition;

	//which means the current symbol is the end of right side
	if (singleProd->right[pos_read + 1] == -1) return true;
	return false;
}

DynamicArray *collectCertainCur_symbol(DynamicArray *prodArray){
	DynamicArray *collectedCur_symbol = createDynamicArray(ARRAY_LENGTH(productions), true, getCur_SymbolForOverlapHandler, INT);
	for (int i = 0; i < getArraySize(prodArray); i++) {
		Production *prod = (Production *)retriveData(prodArray, i, PRODUCTION);
		appendCopy(collectedCur_symbol, prod->cur_symbol, INT);
	}
	return collectedCur_symbol;
}

DynamicArray *gatherProdbyCertainCur_symbol(DynamicArray *prodArray) {
	DynamicArray *collectedCur_symbolArray = gatherCertainCur_symbol(prodArray);
	DynamicArray *collectedProdArray = createDynamicArray(ARRAY_LENGTH(productions), true, getLeftFromProd, PRODUCTION);

	DynamicArray *preDefinedProds = setUpPreDefinedProd();

	Production *expected_data = initializeProduction();

	for (int i = 0; i < getArraySize(collectedCur_symbolArray; i++)) {
		expected_data->left = *(int *)retriveData(collectedCur_symbolArray, i, INT);

		DynamicArray *fetchedProdbySingleSymbolArray = fetchCommonElements(preDefinedProds, cmpLeftFromProd, expected_data, true, PRODUCTION);

		copyPasteArray(collectedProdArray, fetchedProdbySingleSymbolArray);
		destroyDynamicArray(fetchedProdArray);
	}
	free(expected_data);
	return collectedProdArray;
}

Item *findItembyProds(DynamicArray *itemArray, DynamicArray *prodArray) {
	if (getArraySize(itemArray) <= 1) return dymmy_item;
	int expected_hash_key_value = calculateHash(prodArray, getHash_keyOfProdsInItem, PRODUCTION);
	DynamicArray *fetchedItemArray = fetchCommonElements(itemArray, cmpHash_keyOfProdsInItem, ITEM);
	//this is supposed to one item
	DynamicArray *filteredItemArray = fetchCommonElements(fetchedItemArray, cmpReadSymbol, ITEM);
	if (getArraySize(filteredItemArray) != 1) printf("warn: the number of elements: findItembyProds");
	if (getArraySize(filteredItemArray) == 1) return filteredItemArray->data[0];

	return dummy_item;
}

Item *setItem(DynamicArray *itemArray, DynamicArray *fetchedProdArray, int expected_symbol) {
	disableModify(fetchedProdArray);

	DynamicArray *collectedProdArray = gatherProdbyCertainCur_symbol(fetchedProdArray);
	
	printProd(collectedProdArray, PRODUCTION);
	Item *item = findItembyProds(itemArray, collectedProdArray);

	return item;
}

int main() {
	DynamicArray *itemArray = createDynamicArray(20, false, ITEM);
	DynamicArray *initialProdArray = createDynamicArray(10, true, PRODUCTION);
	append(initialProdArray, &productions[0], PRODUCTION);
	Item *first_item = (itemArray, initialProdArray, FIRST);//FIRST temporary
}


//createFirst()
//getFirst()
//createFollow()
//getFollow()
//createState()
//sortRows(
//sortColumns()
//readItem()

