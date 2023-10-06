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
    
	NON_TERMIANL_START = 256,
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
	appendCopied(arr, &productions[key], type);
}

void printProd(DynamicArray *arr, Type type) {
	if (type != PRODUCTION) error("type mismatch: printProd\n");
	
	for (int i = 0; i < getNumElements(arr); i++) {
		Production *prod = (Production *)getData(arr, i, PRODUCTION);
		printf("prod key: %d %p\n", prod->key, &(prod->key));
	}
}

bool isNonTerminal(int symbol) {
    if (symbol >= NON_TERMIANL_START) return true;
    return false;
}

int cur_symbolGetter(Data *prod, Type type) {
	if (type != PRODUCTION) error("type mismatch: cur_symbolGetter\n");
    return ((Production*)prod)->cur_symbol;
}

int leftGetter(Data *prod, Type type) {
	if (type != PRODUCTION) error("type mismatch: leftGetter\n");
    return ((Production*)prod)->left;
}

int getCurSymbol(Data *data) {
    Production *prod = (Production *)(data);
    return prod->cur_symbol;
}

bool cmpSymbol(Data *data, Data *expectedValue) {
    int *symbol = (int *)data;
    int *compedValue = (int *)expectedValue;
    return *symbol == *compedValue;
}

bool isSymbolOverlap(DynamicArray *symbolArr, int symbol, Type type) {
    if (type != INT) error("type mismatch: isSymbolOverlap\n");
    Production *prod;
    for (int i = 0; i < getNumElements(symbolArr); i++) {
        int *existing_symbol = (int *)getData(symbolArr, i, INT);
        if (*existing_symbol == symbol) return true;
    }
    return false;
}

void removeElement(DynamicArray *arr, int i, Type type) {
    if (type != PRODUCTION) error("type mismatch: removeElement\n");
    Production *prod = (Production *)getData(arr, i, PRODUCTION);
    *prod = *(Production *)empty_prod;
}

void prioritizeSymbol(DynamicArray *fetchedSymbolArray, int symbol) {
    int pos_symbol = fetchPosition(fetchedSymbolArray, cmpSymbol, (Data *)&symbol, INT);
    //swap and prioritize the recent element to avoid excess calculate
    if (pos_symbol != 0 && getNumElements(fetchedSymbolArray) > 1) swapWithLastElement(fetchedSymbolArray, pos_symbol, INT);
}

void appendSymbol(DynamicArray *SymbolArray, int cur_symbol) {
	int *copied_symbol = (int *)malloc(sizeof(int *));
	*copied_symbol = cur_symbol;

	append(SymbolArray, copied_symbol, INT);

	prioritizeSymbol(SymbolArray, cur_symbol);
}

void updateCur_Symbol(DynamicArray *prodArr, Type type) {
    if (type != PRODUCTION) error("type mismatch: setSymbol\n");
    for (int i = 0; i < getNumElements(prodArr); i++) {
        Production *prod = (Production *)getData(prodArr, i, PRODUCTION);
        //look ahead and increament
		if (prod->cur_symbol == -1) continue;
        int pos = ++prod->readPosition;
        //update cur_symbol
        int symbol = prod->right[pos];
        prod->cur_symbol = symbol;
		printf("pos symbol cur_symbol at updateCur_symbol(): %d %d %d\n", pos, symbol, cur_symbol);
    }
}


DynamicArray *insertProds() {
	int size = ARRAY_LENGTH(productions);
    DynamicArray *originalProdArray = createDynamicArray(size, true, PRODUCTION);
	
    for (int i = 0; i < size; i++) {
        appendCopiedOriginalProd(originalProdArray, i, PRODUCTION);
    }
	return originalProdArray;
}

DynamicArray *setUpOriginalProd() {
    DynamicArray *originalProdArary = insertProds();

    quickSort(originalProdArary, cur_symbolGetter, 0, getOffset(originalProdArary), PRODUCTION);
    return originalProdArary;
}

//make this recursive function
//this one extract single kind of prods
void appendProdLeftIs(DynamicArray *fetchedProdArray, DynamicArray *originalProdArray, unsigned char *ifExistingArray, int expectedSymbol) {
	int num_original_prod = getNumElements(originalProdArray);

	for (int i = 0; i < num_original_prod; i++) {
		
        Production *prod = (Production *)getData(originalProdArray, i, PRODUCTION);
		if (ifExistingArray[prod->key] == 1) continue;
		
        if (prod->left != expectedSymbol) continue;
		
		ifExistingArray[prod->key] = 1;
		
		Production *copy_prod = (Production *)malloc(getDataSize(PRODUCTION));
		*copy_prod = *prod;
		append(fetchedProdArray, copy_prod, PRODUCTION);//&productions[0] assuming n and the order of elements are sync
		swapRemoveElement(originalProdArray, i--, PRODUCTION);
		
		printf("the num of fetched prod array: %d\n", getNumElements(fetchedProdArray));
		printProd(fetchedProdArray, PRODUCTION);
		
		num_original_prod = getNumElements(originalProdArray);
    }
}

//this one extracts multiple kind of prods
DynamicArray *gatherNessesaryProds(DynamicArray *fetchedProdArray, DynamicArray *fetchedSymbolArray) {
	DynamicArray *originalProdArray = setUpOriginalProd();
	
	//modigy so that this is not needed
	originalProdArray = setUpDupliSortUpdateProd(originalProdArray, PRODUCTION);
	
    int num_elements = getNumElements(fetchedProdArray);
	
	int num_original_prod = getNumElements(originalProdArray);
	
	//second parameter is invalid
	unsigned char *ifExistingArray = setUpUnsignedChar(fetchedProdArray, num_original_prod, type);
	
	
    for (int i = 0; i < num_elements; i++) {

        Production *prod = (Production *)getData(fetchedProdArray, i, PRODUCTION);
                        
        int cur_symbol = prod->cur_symbol;

        if (!isNonTerminal(cur_symbol) || isSymbolOverlap(fetchedSymbolArray, cur_symbol, INT)) continue;
        
		appendSymbol(fetchedSymbolArray, cur_symbol);
		
        appendProdLeftIs(fetchedProdArray, originalProdArray, ifExistingArray, cur_symbol);
        
        num_elements = getNumElements(fetchedProdArray);
        
    }
	
    return fetchedProdArray;
}

/*
 ------  transistedProds_Item  ------
 |create  |-----------------|create  |
 |Item    |      Symbol     |Item    |
  ---------                  --------
 */
DynamicArray *setUpDupliSortUpdateProd(DynamicArray *oldProdArray, Type type) {
    if (type != PRODUCTION) error("type mismatch: setSymbol\n");
    DynamicArray *duplicatedProdsArray = duplicateArray(oldProdArray, true);
    quickSort(duplicatedProdsArray, leftGetter, 0, getOffset(duplicatedProdsArray), PRODUCTION);
    updateCur_Symbol(duplicatedProdsArray, PRODUCTION);
    return duplicatedProdsArray;
}

//make this optimize later particulary using deprioritizer
bool isEndProd(DynamicArray *prodArr) {
    if (getNumElements(prodArr) != 1) return false;
    Production *singleProd = (Production *)getData(prodArr, 0, PRODUCTION);
    int pos_read = singleProd->readPosition;
    
    //which means the current symbol is the end of right side
    if (singleProd->right[pos_read + 1] == -1) return true;
    return false;
}

//you may be able to find faster way other than deprioritizer
DynamicArray *extractProd(DynamicArray *copiedProdArray, int expectedSymbol) {
    DynamicArray *extractedProdArr = createDynamicArray(getNumElements(copiedProdArray), false, PRODUCTION);
    for (int i = 0; i < getNumElements(copiedProdArray); i++){
        Production *copiedProd = (Production *)getData(copiedProdArray, i, PRODUCTION);
        int sym = copiedProd->cur_symbol;
        
        if (sym != expectedSymbol) continue;
        //copied one will be deprioritized thats why you use the original one
        Production *new_copiedProd = (Production *)malloc(sizeof(copiedProd));
        *new_copiedProd = *copiedProd;
        append(extractedProdArr, new_copiedProd, PRODUCTION);
        swapRemoveElement(copiedProdArray, i, PRODUCTION);
        }
    return extractedProdArr;
}

//make this optimize later particulary using deprioritizer
Item *transistedProds_Item(DynamicArray *itemArray, DynamicArray *ProdArray, DynamicArray *symbolArray) {

    //eliminateOverlap(gatheredProdArray, getKey, PRODUCTION);
    DynamicArray *newItems = createDynamicArray(getNumElements(symbolArray) * getDataSize(ITEM), false, ITEM);
    
    //this part is supposed to be done before createItem() below
    int cur_latest_num_item = getOffset(itemArray);
    for (int i = 0; i < getNumElements(ProdArray); i++) {
        Production *prod = (Production *)getData(ProdArray, i, PRODUCTION);
		int symbol = prod->cur_symbol;
		
		if (symbol == END) continue;
        
        if (symbol == -1) continue;
		
        DynamicArray *extractedProdArr = extractProd(ProdArray, symbol);
		
        if (getNumElements(extractedProdArr) == 0) continue;
        
        if (isEndProd(extractedProdArr)) continue;
        Item *part_new_item = createItem(itemArray, extractedProdArr, symbol);
        append(newItems, part_new_item, ITEM);
    }

    //destroyDynamicArray(newProdArr);
	return (Item *)newItems;
}

//this take much time
bool isExsistingItem(Item *referentItem, Production *prod) {
    int value_hashed_key = referentItem->hashed_keys;
    Production *referentProd = getProd_Item(referentItem, ITEM);
    if (calculateSetHash((DynamicArray *)referentProd, getKey, PRODUCTION) == value_hashed_key) return true;
    
    return false;
}

Item *fetchItem(DynamicArray *itemArray, DynamicArray *prodArray, int expectedSymbol) {
	//make this outside
    if (getNumElements(itemArray) == 0) return dummy_item;
    Item *latest_arr = (Item *)getData(itemArray, getOffset(itemArray), ITEM);
    DynamicArray *pos_items = fetchMultiPositions(itemArray, cmpTransitionedSymbol, (Data *)&expectedSymbol, ITEM);
    for (int i = 0; i < getNumElements(pos_items); i++) {
        Item *item = (Item *)getData(itemArray, i, ITEM);//sus *pos - 1
        if (latest_arr->transitionedSymbol != expectedSymbol) continue;
        if (isExsistingItem(item, (Production *)prodArray)) return item;
    }
    destroyDynamicArray(pos_items);
	
    return dummy_item;
}

//the copying prod takes much time
Item *createItem(DynamicArray *itemArray, DynamicArray *fetchedProdArray, int expectedSymbol) {
	printf("createItem: %d\n", getNumElements(itemArray));
    DynamicArray *duplicatedProdArray = setUpDupliSortUpdateProd(fetchedProdArray, PRODUCTION);
    Item *existingItem = fetchItem(itemArray, duplicatedProdArray, expectedSymbol);
	if (existingItem->stateId != -1) {
        return existingItem;
    } else {
        DynamicArray *fetchedSymbolArray = createDynamicArray(10, true, INT);

		DynamicArray *gatheredProdArray = gatherNessesaryProds(duplicatedProdArray, fetchedSymbolArray);
		
		Item *cur_latest_item = setItem(itemArray, getNumElements(itemArray), expectedSymbol, gatheredProdArray);

        Item *newItems = transistedProds_Item(itemArray, gatheredProdArray, fetchedSymbolArray);
        cur_latest_item->transitionDestinations = newItems;
		
		free(duplicatedProdArray);
		free(fetchedSymbolArray);
        return cur_latest_item;
    }
}

int main() {
    // Test Case 1: Creating and appending elements to a dynamic array
    DynamicArray *arr = createDynamicArray(5, true, false, INT);
    int element1 = 10;
    int element2 = 20;
    int element3 = 30;
    append(arr, &element1, INT);
    append(arr, &element2, INT);
    append(arr, &element3, INT);

    // Test Case 2: Swapping elements in the dynamic array
    printf("Original Array: ");
    for (int i = 0; i <= getOffset(arr); ++i) {
        int *value = (int *)getData(arr, i, INT);
        printf("%d ", *value);
    }
    printf("\n");

    swapElement(arr, 0, 2, INT);

    printf("Array after swapping: ");
    for (int i = 0; i <= getOffset(arr); ++i) {
        int *value = (int *)getData(arr, i, INT);
        printf("%d ", *value);
    }
    printf("\n");

    // Test Case 3: Sorting elements in the dynamic array
    printf("Array before sorting: ");
    for (int i = 0; i <= getOffset(arr); ++i) {
        int *value = (int *)getData(arr, i, INT);
        printf("%d ", *value);
    }
    printf("\n");

    quickSort(arr, getKey, 0, getOffset(arr), INT);

    printf("Array after sorting: ");
    for (int i = 0; i <= getOffset(arr); ++i) {
        int *value = (int *)getData(arr, i, INT);
        printf("%d ", *value);
    }
    printf("\n");

    // Test Case 4: Cleaning up and destroying the dynamic array
    destroyDynamicArray(arr);

    return 0;
}
/*
int main() {
    DynamicArray *itemArray = createDynamicArray(10, false, ITEM);
    DynamicArray *fetchedProdArray = createDynamicArray(10, true, PRODUCTION);
    append(fetchedProdArray, &productions[0], PRODUCTION);
    Item *first_item = createItem(itemArray, fetchedProdArray, FIRST);//FIRST temporary
}
*/

//createFirst()
//getFirst()
//createFollow()
//getFollow()
//createState()
//sortRows(
//sortColumns()
//readItem()

