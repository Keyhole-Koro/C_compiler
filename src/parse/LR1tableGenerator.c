#include "LR1tableGenerator.h"

void printProd(DynamicArray *arr) {
    if (arr->type->name != "production") error("type mismatch: printProd\n");

    printf("--------\n|inside of a prod\n");
    printf("|-size: %d\n", getArraySize(arr));
    printf("|-overlap: ");
    arr->ifAllowOverlap ? printf("allowed\n"):printf("not alloweded\n");
    printf("|-modifing: ");
    arr->allowModifying ? printf("allowed\n") : printf("not allowed\n");
    for (int i = 0; i < getArraySize(arr); i++) {
        Production *prod = (Production *)retriveData(arr, i, PRODUCTION);
        printf("|prod key left readPosition cur_symbol: %d %d %d %d\n", prod->key, getLeftFromProd((Data*)prod, PRODUCTION), prod->readPosition, prod->cur_symbol);
    }
    printf("--------\n");
}

bool if_prodsSame(Production *prod1, Production *prod2) {
    unsigned char *array = createUnsignedCharArray(ARRAY_LENGTH(preset_expr_instance));

    for (int i = 0; i < ARRAY_LENGTH(prod1); i++) {
        array[prod1[i].key] = 1;
    }   
    for (int i = 0; i < ARRAY_LENGTH(prod2); i++) {
        if (array[prod2[i].key] != 1) return false;
        array[prod2[i].key] = 1;
    }
    return true;
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

state_id makeNewStateId(DynamicArray *itemArray) {
    if (itemArray->type->name != "item") error("type mismatch: makeNewStateId");
    return getArraySize(itemArray);
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
	if (prodArr->type->name != "production") error("type mismatch: setSymbol\n");
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
	if (type->name != "production") error("type mismatch: setSymbol\n");
	DynamicArray *clonedProdsArray = cloneArray(oldProdArray, true, oldProdArray->referentMember);
	//quickSort(clonedProdsArray, getLeftFromProd, 0, getArrayOffset(clonedProdsArray), PRODUCTION);
    readOneSymbol(clonedProdsArray);
	return clonedProdsArray;
}

bool isClosureItem(DynamicArray *prodArr) {
    if (prodArr->type->name != "production") error("type mismatch: setSymbol\n");
	if (getArraySize(prodArr) != 1) return false;
	Production *singleProd = (Production *)retriveData(prodArr, 0, PRODUCTION);

	//if the current symbol is the end of right side
	if (singleProd->cur_symbol == NONE) return true;
	return false;
}

DynamicArray *extract_curSymbol(DynamicArray *prodArray, DynamicArray *appendedArray, bool ifAllowTerminal){
	if (prodArray->type->name != "production") printf("type mismatch: listCur_symbol\n");
    if (appendedArray->type->name != INT) printf("type mismatch: listCur_symbol\n");

    for (int i = 0; i < getArraySize(prodArray); i++) {
		Production *prod = (Production *)retriveData(prodArray, i, PRODUCTION);
        if (!ifAllowTerminal && !isNonTerminal(prod->cur_symbol)) continue;
        appendCopy(appendedArray, &(prod->cur_symbol), INT);
	}

    return appendedArray;
}

DynamicArray *listCur_symbol(DynamicArray *prodArray, bool ifAllowTerminal){

    DynamicArray *listedCur_symbolArray = createDynamicArray(ARRAY_LENGTH(preset_expr_instance), true, getIntFromDataForSymbol, INT);

    return extract_curSymbol(prodArray, listedCur_symbolArray, ifAllowTerminal);;
}

bool ifItemExists(DynamicArray *itemArray, DynamicArray *prodArray, int expected_readSymbol) {
    if (getArraySize(itemArray) == 0) return false;

    Item *expected_item = initializeItem();
    expected_item->readSymbol = expected_readSymbol;
    expected_item->hashed_keys = calculateHash(prodArray, getKeyFromProd, PRODUCTION);

    DynamicArray *fetchedItemArray = fetchCommonElements(itemArray, cmp_readSymbolHash_keyOfProdsInItem, (Data *)expected_item, false, ITEM);
    /*printf("item------\n");
    for (int i = 0; i < getArraySize(fetchedItemArray); i++) {
        printf("i: %d\n", (Item*)retriveData(fetchedItemArray, i, ITEM)->stateId);
    }*/
    if (!if_prodsSame((Production*)(prodArray->data), (Production*)(fetchedItemArray->data)))
    //this is supposed to exist just one item
    if (getArraySize(fetchedItemArray) >= 2) error("fetchedItemArray is supposed to have one element");
    printf("getArraySize(fetchedItemArray): %d\n", getArraySize(fetchedItemArray));

    if (getArraySize(fetchedItemArray) == 1) return true;

    return false;
}

state_id processNonTerminalSymbols(DynamicArray *itemArray, DynamicArray *prodArray){
    DynamicArray *listedCur_symbolArray = listCur_symbol(prodArray, true);

    Production *expected_prod = initializeProduction();

    state_id hashed_stateId = 0;

    DynamicArray *clonedProdarray = cloneArray(prodArray, true, prodArray->referentMember);

    for(int i = 0; i < getArraySize(listedCur_symbolArray); i++) {
        int expected_symbol = *(int *)retriveData(listedCur_symbolArray, i, INT);
        expected_prod->cur_symbol = expected_symbol;

        DynamicArray *fetchedProdbySingleSymbolArray = fetchCommonElements(clonedProdarray, cmpCur_symbol, (Data *)expected_prod, true, PRODUCTION);

        readOneSymbol(fetchedProdbySingleSymbolArray);

        int new_stateId = constructItem(itemArray, fetchedProdbySingleSymbolArray, expected_symbol);
        if (new_stateId != -1) hashed_stateId ^= new_stateId;
    }
    free(expected_prod);
    
    return hashed_stateId;
}

DynamicArray *gatherProdswithSymbols(DynamicArray *prodArray) {    
    DynamicArray *listedCur_symbolArray = listCur_symbol(prodArray, false);
    
    DynamicArray *presetProdArray = presetProductions();

    Production *expected_prod = initializeProduction();
    int condition = getArraySize(listedCur_symbolArray);

    for (int i = 0; i < condition; i++) {
        expected_prod->expr->left = *(int *)retriveData(listedCur_symbolArray, i, INT);

        DynamicArray *newly_fetchedProdbySingleSymbolArray = fetchCommonElements(presetProdArray, cmpLeftFromProd, (Data *)expected_prod, true, PRODUCTION);

        extract_curSymbol(newly_fetchedProdbySingleSymbolArray, listedCur_symbolArray, true);

        copyPasteArray(newly_fetchedProdbySingleSymbolArray, prodArray);

        destroyDynamicArray(newly_fetchedProdbySingleSymbolArray);
        
        condition = getArraySize(listedCur_symbolArray);
    }
    free(expected_prod);
}

state_id constructItem(DynamicArray *itemArray, DynamicArray *prodArray, int expected_symbol) {
	gatherProdswithSymbols(prodArray);

	if (ifItemExists(itemArray, prodArray, expected_symbol)) return -1 ;//when the item exists

    state_id new_stateId = makeNewStateId(itemArray);

    printf("size: %d\n", getArraySize(itemArray));
    printProd(prodArray);
    
	Item *item = createItem(new_stateId, expected_symbol, getArraySize(prodArray), (Production **)(prodArray->data), calculateHash(prodArray, getKeyFromProd, PRODUCTION));
	append(itemArray, item, ITEM);
    
    disableModifying(prodArray);

	if (!isClosureItem(prodArray)) item->hashed_keys = processNonTerminalSymbols(itemArray, prodArray);

    return new_stateId;
}


int main() {
	DynamicArray *itemArray = createDynamicArray(20, true, dummy_member, ITEM);
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

