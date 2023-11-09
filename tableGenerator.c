#include "tableGenerator.h"

enum {
    NONE = 1,
	TERMIANL_START = 10,
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
    END_TERMINAL = 30,
	NON_TERMINAL_START = 256,
	EXPRRESSION, // E
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
Expr preset_expr_instance[] = {
	{0, EXPRRESSION, {EXPRRESSION, END, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{1, EXPRRESSION, {EXPRRESSION, ADD, TERM, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{2, EXPRRESSION, {EXPRRESSION, SUB, TERM, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{3, EXPRRESSION, {TERM, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{4, TERM, {TERM, MUL, FACTOR, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{5, TERM, {TERM, DIV, FACTOR, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{6, TERM, {FACTOR, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{7, FACTOR, {L_PARENTHESES, EXPRRESSION, R_PARENTHESES, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{8, FACTOR, {NUM, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
};

Expr *preset_expr = preset_expr_instance;

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
    int sym = *(int *)data;
    if (sym > NON_TERMINAL_START) return abs(sym - NON_TERMINAL_START) + END_TERMINAL;
    if (TERMIANL_START < sym && sym < END_TERMINAL) return sym;
    return -1;
}

int getKeyFromProd(Data *data, Type type) {
	if (type != PRODUCTION) error("type mismatch: getKeyFromProd\n");
	return ((Production*)data)->key;
}

int getLeftFromProd(Data *data, Type type) {
	if (type != PRODUCTION) error("type mismatch: getLeftFromProd\n");
	return ((Production*)data)->expr->left;
}

int getRightFromProd(Data *data, int index, Type type) {
    if (type != PRODUCTION) error("type mismatch: getLeftFromProd\n");
    return ((Production*)data)->expr->right[index];
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

void updateCur_symbol(Production *prod) {
    prod->cur_symbol = getRightFromProd((Data*)prod, prod->readPosition, PRODUCTION);
}

int lookAheadSymbol(Production *prod) {
    return prod->expr->right[prod->readPosition + 1];
}

Expr *getExprWithKey(int key) {
    //assuming they are arranged corresponding to the same number
    if (preset_expr[key].key == key) return &preset_expr[key];
    //just in case
    for (int i = 0; i < ARRAY_LENGTH(preset_expr_instance); i++) {
        if (preset_expr[i].key == key) return &preset_expr[i];
    }
    error("expr not found");
    return dummy_expr;
}
DynamicArray *presetProductions() {
    int size = ARRAY_LENGTH(preset_expr_instance);
    DynamicArray *originalProdArray = createDynamicArray(size, true, getKeyFromProd, PRODUCTION);

    Production *prod = initializeProduction();
    for (int i = 0; i < size; i++) {
        prod->key = i;
        prod->expr = getExprWithKey(i);
        prod->readPosition = 0;
        updateCur_symbol(prod);
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
		++prod->readPosition;
		//update cur_symbol
        updateCur_symbol(prod);
	}
}

DynamicArray *clone_readOne_Cur_symbol(DynamicArray *oldProdArray, Type type) {
	if (type != PRODUCTION) error("type mismatch: setSymbol\n");
	DynamicArray *clonedProdsArray = cloneArray(oldProdArray, true, oldProdArray->referentMember);
	//quickSort(clonedProdsArray, getLeftFromProd, 0, getArrayOffset(clonedProdsArray), PRODUCTION);
    readOneSymbol(clonedProdsArray);
	return clonedProdsArray;
}

bool isClosureItem(DynamicArray *prodArr) {
    if (prodArr->type != PRODUCTION) error("type mismatch: setSymbol\n");
	if (getArraySize(prodArr) != 1) return false;
	Production *singleProd = (Production *)retriveData(prodArr, 0, PRODUCTION);

	//if the current symbol is the end of right side
	if (singleProd->cur_symbol == NONE) return true;
	return false;
}

DynamicArray *extract_appendCur_symbol(DynamicArray *prodArray, DynamicArray *appendedArray, bool ifAllowTerminal){
	if (prodArray->type != PRODUCTION) printf("type mismatch: listCur_symbol\n");
    if (appendedArray->type != INT) printf("type mismatch: listCur_symbol\n");

    for (int i = 0; i < getArraySize(prodArray); i++) {
		Production *prod = (Production *)retriveData(prodArray, i, PRODUCTION);
        if (!ifAllowTerminal && !isNonTerminal(prod->cur_symbol)) continue;
        appendCopy(appendedArray, &(prod->cur_symbol), INT);
	}

    return appendedArray;
}

DynamicArray *extract_listCur_symbol(DynamicArray *prodArray, bool ifAllowTerminal){

    DynamicArray *listedCur_symbolArray = createDynamicArray(ARRAY_LENGTH(preset_expr_instance), true, getIntFromDataForSymbol, INT);

    return extract_appendCur_symbol(prodArray, listedCur_symbolArray, ifAllowTerminal);;
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
    DynamicArray *listedCur_symbolArray = extract_listCur_symbol(prodArray, true);

    Production *expected_prod = initializeProduction();

    state_id hashed_stateId = 0;

    DynamicArray *clonedProdarray = cloneArray(prodArray, true, dummy_member);

    for(int i = 0; i < getArraySize(listedCur_symbolArray); i++) {
        int expected_symbol = *(int *)retriveData(listedCur_symbolArray, i, INT);
        expected_prod->cur_symbol = expected_symbol;

        DynamicArray *fetchedProdbySingleSymbolArray = fetchCommonElements(clonedProdarray, cmpCur_symbol, (Data *)expected_prod, true, PRODUCTION);

        readOneSymbol(fetchedProdbySingleSymbolArray);

        hashed_stateId ^= constructItem(itemArray, fetchedProdbySingleSymbolArray, expected_symbol);
    }
    free(expected_prod);
    
    return hashed_stateId;
}

DynamicArray *gatherProdswithSymbols(DynamicArray *prodArray) {
    DynamicArray *listedCur_symbolArray = extract_listCur_symbol(prodArray, false);
    
    DynamicArray *presetProdArray = presetProductions();

    Production *expected_prod = initializeProduction();
    int condition = getArraySize(listedCur_symbolArray);

    for (int i = 0; i < condition; i++) {
        expected_prod->expr->left = *(int *)retriveData(listedCur_symbolArray, i, INT);

        DynamicArray *newly_fetchedProdbySingleSymbolArray = fetchCommonElements(presetProdArray, cmpLeftFromProd, (Data *)expected_prod, true, PRODUCTION);

        extract_appendCur_symbol(newly_fetchedProdbySingleSymbolArray, listedCur_symbolArray, true);

        copyPasteArray(newly_fetchedProdbySingleSymbolArray, prodArray);
        destroyDynamicArray(newly_fetchedProdbySingleSymbolArray);
        
        condition = getArraySize(listedCur_symbolArray);
    }
    free(expected_prod);
    return prodArray;
}

state_id constructItem(DynamicArray *itemArray, DynamicArray *prodArray, int expected_symbol) {
	disableModify(prodArray);

	DynamicArray *collectedProdArray = gatherProdswithSymbols(prodArray);
    
	Item *existingItem = findExistingItem(itemArray, collectedProdArray, expected_symbol);
	if (existingItem != dummy_item) return -1 ;//if the item exists

    state_id new_stateId = getArraySize(itemArray);

    printf("size: %d\n", getArraySize(itemArray));
    printProd(collectedProdArray);
    
	Item *item = createItem(new_stateId, expected_symbol, getArraySize(collectedProdArray), (Production **)(collectedProdArray->data), calculateHash(collectedProdArray, getKeyFromProd, PRODUCTION));
	append(itemArray, item, ITEM);
    
    disableModify(collectedProdArray);

	if (!isClosureItem(collectedProdArray)) item->hashed_keys = separateProds(itemArray, collectedProdArray);

    return new_stateId;
}


int main() {
	DynamicArray *itemArray = createDynamicArray(20, false, dummy_member, ITEM);
	DynamicArray *initialProdArray = createDynamicArray(10, true, getKeyFromProd, PRODUCTION);
    DynamicArray *preset_prodArray = presetProductions();
	appendCopy(initialProdArray, retriveData(preset_prodArray, 0, PRODUCTION), PRODUCTION);

    constructItem(itemArray, initialProdArray, FIRST);//FIRST temporary
    printf("size of item: %d\n", getArraySize(itemArray));
    /*
    for (int i = 0; i < getArraySize(itemArray); i++) {
        Item *item = (Item *)retriveData(itemArray, i, ITEM);
        Production **prods = item->Productions;
        printf("--------\n");
        printf("size item->arraySize %d\n", item->arraySize);
        for (int j = 0; j < item->arraySize; j++) {
            Production *prod = prods[j];
            printf("|prod key left readPosition cur_symbol: %d %d %d %d\n", prod->key, getLeftFromProd((Data*)prod, PRODUCTION), prod->readPosition, prod->cur_symbol);
        }
        printf("--------\n");
    }
     */
}


//createFirst()
//getFirst()
//createFollow()
//getFollow()
//createState()
//sortRows(
//sortColumns()
//readItem()

