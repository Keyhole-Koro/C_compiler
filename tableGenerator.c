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
    NONE,
};

//pre defined Productions
//{EXPR, ADD, TERM}
//   0    1     2
Expr expr_instance[] = {
	{0, EXPR, {EXPR, END, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{1, EXPR, {EXPR, ADD, TERM, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{2, EXPR, {EXPR, SUB, TERM, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{3, EXPR, {TERM, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{4, TERM, {TERM, MUL, FACTOR, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{5, TERM, {TERM, DIV, FACTOR, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{6, TERM, {FACTOR, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{7, FACTOR, {L_PARENTHESES, EXPR, R_PARENTHESES, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{7, FACTOR, {NUM, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
};

Expr *preset_expr = expr_instance;

void printProd(DynamicArray *arr) {
    if (arr->type != PRODUCTION) error("type mismatch: printProd\n");

    printf("--------\n|inside of a prod\n");
    printf("|-size: %d\n", getArraySize(arr));
    printf("|-overlap: ");
    arr->ifAllowOverlap ? printf("allowed\n"):printf("not alloweded\n");
    printf("|-modifing: ");
    arr->allowModify ? printf("allowed\n") : printf("not allowed\n");
    for (int i = 0; i < getArraySize(arr); i++) {
        Production *prod = (Production *)retriveData(arr, i, PRODUCTION);
        printf("|prod key left readPosition cur_symbol: %d %d %d %d\n", prod->key, getLeftFromProd((Data*)prod, PRODUCTION), prod->readPosition, prod->cur_symbol);
    }
    printf("--------\n");
}

bool isNonTerminal(int symbol) {
	return symbol > NON_TERMINAL_START;
}

int getIntFromData(Data *data, Type type) {
	if (type != INT) error("type mismatch: getIntFromData\n");
	return *(int *)data;
}

int getIntFromDataForSymbol(Data *data, Type type) {
	if (type != INT) error("type mismatch: getIntFromDataForSymbol\n");
	return *(int *)data - NON_TERMINAL_START;
}

int getKeyFromProd(Data *data, Type type) {
	if (type != PRODUCTION) error("type mismatch: getKeyFromProd\n");
	return ((Production*)data)->key;
}

int getLeftFromProd(Data *data, Type type) {
	if (type != PRODUCTION) error("type mismatch: getLeftFromProd\n");
	return ((Production*)data)->expr->left;
}

int getCur_symbol(Data *data, Type type) {
	if (type != PRODUCTION) error("type mismatch: getCur_symbol\n");
	Production *prod = (Production *)(data);
	return prod->cur_symbol;
}

int getCur_symbolForOverlapHandler(Data *data, Type type) {
	if (type != PRODUCTION && type != INT) error("type mismatch: getCur_symbolForOverlapHandler\n");
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

bool cmp_readSymbolHash_keyOfProdsInItem(Data *data, Data *expectedData, Type type) {
	if (type != ITEM) error("type mismatch: cmpHash_keyOfProdsInItem\n");
	Item *item = (Item *)data;
	Item *cmpedItem = (Item *)expectedData;
	if (item->hashed_keys != cmpedItem->hashed_keys) return false;
	if (item->readSymbol != cmpedItem->readSymbol) return false;
	return true;
}

int getHash_keyOfProdsInItem(Data *data, Type type) {
	if (type != ITEM) error("type mismatch: getHash_keyOfProdsInItem\n");
	Item *item = (Item *)data;
	return item->hashed_keys;
}

bool cmpLeftFromProd(Data *data, Data *expectedValue, Type type) {
	if (type != PRODUCTION) error("type mismatch: cmpLefFromProd\n");
	return getLeftFromProd(data, PRODUCTION) == getLeftFromProd(expectedValue, PRODUCTION);
}

bool cmpStateId(Data *data, Data *expectedValue, Type type) {
	if (type != ITEM) error("type mismatch: cmpStateId\n");
	Item *item = (Item *)data;
	int *compedValue = (int*)expectedValue;
	return item->stateId == *compedValue;
}

bool cmpReadSymbol(Data* data, Data* expectedValue, Type type) {
	if (type != ITEM) error("type mismatch: cmpReadSymbol\n");
	Item *item = (Item *)data;
	int *compedValue = (int*)expectedValue;
	return item->readSymbol == *compedValue;
}

Expr *getExprWithKey(int key) {
    //assuming they are arranged corresponding to the same number
    if (preset_expr[key].key == key) return &preset_expr[key];
    //just in case
    for (int i = 0; i < ARRAY_LENGTH(preset_expr); i++) {
        if (preset_expr[i].key == key) return preset_expr[i];
    }
}
DynamicArray *presetProductions() {
    int size = ARRAY_LENGTH(preset_expr);
    DynamicArray *originalProdArray = createDynamicArray(size, true, dummy_member, PRODUCTION);

    Production *prod = initializeProduction();
    for (int i = 0; i < size; i++) {
        prod->key = i;
        prod->expr = getExprWithKey(i);
        prod->readPosition = 0;
        prod->cur_symbol = prod->expr->left[prod->readPosition];
        appendCopy(originalProdArray, prod, PRODUCTION);
    }
    return originalProdArray;
}

void readOneSymbol(DynamicArray *prodArr) {
	if (prodArr->type != PRODUCTION) error("type mismatch: setSymbol\n");
	for (int i = 0; i < getArraySize(prodArr); i++) {
		Production *prod = (Production *)retriveData(prodArr, i, PRODUCTION);
		
		if (prod->cur_symbol == NONE) continue;
        //look ahead and increament
		int pos = ++prod->readPosition;
		//update cur_symbol
		int symbol = prod->expr[pos];
		prod->cur_symbol = symbol;
	}
}

DynamicArray *Clone_readOne_Cur_symbol(DynamicArray *oldProdArray, Type type) {
	if (type != PRODUCTION) error("type mismatch: setSymbol\n");
	DynamicArray *clonedProdsArray = cloneArray(oldProdArray, true, oldProdArray->referentMember);
	//quickSort(clonedProdsArray, getLeftFromProd, 0, getArrayOffset(clonedProdsArray), PRODUCTION);
	updateCur_Symbol(clonedProdsArray, type);
	return clonedProdsArray;
}

bool isClosureItem(DynamicArray *prodArr) {
    if (prodArr->type != PRODUCTION) error("type mismatch: setSymbol\n");
	if (getArraySize(prodArr) != 1) return false;
	Production *singleProd = (Production *)retriveData(prodArr, 0, PRODUCTION);
	int pos_read = singleProd->readPosition;

	//which means the current symbol is the end of right side
	if (singleProd->right[pos_read + 1] == NONE) return true;
	return false;
}

DynamicArray *extract_appnedCur_symbol(DynamicArray *prodArray, DynamicArray *appendedArray, bool ifAllowNonTerminal){
	if (prodArray->type != PRODUCTION) printf("type mismatch: listCur_symbol\n");
    if (appendedArray->type != INT) printf("type mismatch: listCur_symbol\n");

	for (int i = 0; i < getArraySize(prodArray); i++) {
		Production *prod = (Production *)retriveData(prodArray, i, PRODUCTION);

		if (!ifAllowNonTerminal && !isNonTerminal(prod->cur_symbol)) continue;

		appendCopy(appendedArray, &(prod->cur_symbol), INT);
	}
	return appendedArray;
}

DynamicArray *extract_listCur_symbol(DynamicArray *prodArray, bool ifAllowNonTerminal){
    DynamicArray *listedCur_symbolArray = createDynamicArray(ARRAY_LENGTH(preset_expr), true, getIntFromDataForSymbol, INT);
    return extract_appnedCur_symbol(prodArray, listedCur_symbolArray, ifAllowNonTerminal);;
}

Item *findExistingItem(DynamicArray *itemArray, DynamicArray *prodArray, int expected_readSymbol) {
    if (getArraySize(itemArray) == 0) return dummy_item;

    Item *expected_item = initializeItem();
    expected_item->readSymbol = expected_readSymbol;
    expected_item->hashed_keys = calculateHash(prodArray, getKeyFromProd, PRODUCTION);

    DynamicArray *fetchedItemArray = fetchCommonElements(itemArray, cmp_readSymbolHash_keyOfProdsInItem, (Data *)expected_item, false, ITEM);
    //this is supposed to exist just one item
    if (getArraySize(fetchedItemArray) >= 2) printf("fetchedItemArray isnt supposed to have %d elements", getArraySize(fetchedItemArray));
    if (getArraySize(fetchedItemArray) == 1) return (Item *)retriveData(fetchedItemArray, 0, ITEM);

    return dummy_item;
}

state_id separateProds(DynamicArray *itemArray, DynamicArray *prodArray){
    DynamicArray *listedCur_symbolArray = listCur_symbol(prodArray, true);

    Production *expected_data = initializeProduction();

    state_id hashed_stateId = 0;
    
    for(int i = 0; i < getArraySize(listedCur_symbolArray); i++) {\
        expected_data->cur_symbol = *(int *)retriveData(listedCur_symbolArray, i, INT);

        DynamicArray *fetchedProdbySingleSymbolArray = fetchCommonElements(prodArray, cmpCur_symbol, (Data *)expected_data, true, PRODUCTION);
        if (!isClosureItem(fetchedProdbySingleSymbolArray)) hashed_stateId ^= constructItem(itemArray, fetchedProdbySingleSymbolArray, expected_symbol);
    }
    free(expected_data);
    
    return hashed_stateId;
}

DynamicArray *gatherProdswithSymbols(DynamicArray *prodArray) {
    DynamicArray *listedCur_symbolArray = extract_listCur_symbol(prodArray, false);

    
    DynamicArray *presetProdArray = presetProductions();
    Production *expected_data = initializeProduction();
    int condition = getArraySize(listedCur_symbolArray);
    
    for (int i = 0; i < condition; i++) {
        expected_data->left = *(int *)retriveData(prodArray, i, INT);
        
        DynamicArray *newly_fetchedProdbySingleSymbolArray = fetchCommonElements(presetProdArray, cmpLeftFromProd, (Data *)expected_data, true, PRODUCTION);

        extract_appendCur_symbol(fetchedProdbySingleSymbolArray, listedCur_symbolArray, true);
        
        copyPasteArray(newly_fetchedProdbySingleSymbolArray, collectedProdArray);
        destroyDynamicArray(newly_fetchedProdbySingleSymbolArray);
        
        condition = getArraySize(listedCur_symbolArray);
    }
    free(expected_data);
    return prodArray;
}

state_id constructItem(DynamicArray *itemArray, DynamicArray *prodArray, int expected_symbol) {
	disableModify(prodArray);

	DynamicArray *collectedProdArray = gatherProdswithSymbols(prodArray);

	Item *existingItem = findExistingItem(itemArray, collectedProdArray, expected_symbol);
	if (existingItem != dummy_item) return;//if the item exists

    state_id new_stateId = getArraySize(itemArray);
    
	Item *item = createItem(new_stateId, expected_symbol, (Production *)(fetchedProdArray->data), calculateHash(fetchedProdArray, getKeyFromProd, PRODUCTION), -1);
	append(itemArray, item, ITEM);
	    
    DynamicArray *cloned_updatedProdArray = Clone_readOne_Cur_symbol(collectedProdArray, PRODUCTION);
	if (!isClosureItem(collectedProdArray)) item->hashed_keys = separateProds(itemArray, clonedCollectedProdArray);
    
    return new_stateId;
}


int main() {
	DynamicArray *itemArray = createDynamicArray(20, false, dummy_member, ITEM);
	DynamicArray *initialProdArray = createDynamicArray(10, true, getKeyFromProd, PRODUCTION);
    Production *preset_prods = presetProductions();
	appendCopy(initialProdArray, getData(preset_prods, 0, PRODUCTION), PRODUCTION);
	constructItem(itemArray, initialProdArray, FIRST);//FIRST temporary
}


//createFirst()
//getFirst()
//createFollow()
//getFollow()
//createState()
//sortRows(
//sortColumns()
//readItem()

