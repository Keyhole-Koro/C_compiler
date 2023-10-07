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
	appendCopy(arr, &productions[key], type);
}

void printProd(DynamicArray *arr, Type type) {
	if (type != PRODUCTION) error("type mismatch: printProd\n");
	
	for (int i = 0; i < getArraySize(arr); i++) {
		Production *prod = (Production *)retriveData(arr, i, PRODUCTION);
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
    for (int i = 0; i < getArraySize(symbolArr); i++) {
        int *existing_symbol = (int *)retriveData(symbolArr, i, INT);
        if (*existing_symbol == symbol) return true;
    }
    return false;
}

void removeElement(DynamicArray *arr, int i, Type type) {
    if (type != PRODUCTION) error("type mismatch: removeElement\n");
    Production *prod = (Production *)retriveData(arr, i, PRODUCTION);
    *prod = (Production)*dummy_prod;
}

void prioritizeSymbol(DynamicArray *fetchedSymbolArray, int symbol) {
    int pos_symbol = fetchPosition(fetchedSymbolArray, cmpSymbol, (Data *)&symbol, INT);
    //swap and prioritize the recent element to avoid excess calculate
    if (pos_symbol != 0 && getArraySize(fetchedSymbolArray) > 1) swapWithLastElement(fetchedSymbolArray, pos_symbol, INT);
}

void appendSymbol(DynamicArray *SymbolArray, int cur_symbol) {
	int *copied_symbol = (int *)malloc(sizeof(int *));
	*copied_symbol = cur_symbol;

	append(SymbolArray, copied_symbol, INT);

	prioritizeSymbol(SymbolArray, cur_symbol);
}

void updateCur_Symbol(DynamicArray *prodArr, Type type) {
    if (type != PRODUCTION) error("type mismatch: setSymbol\n");
    for (int i = 0; i < getArraySize(prodArr); i++) {
        Production *prod = (Production *)retriveData(prodArr, i, PRODUCTION);
        //look ahead and increament
		if (prod->cur_symbol == -1) continue;
        int pos = ++prod->readPosition;
        //update cur_symbol
        int symbol = prod->right[pos];
        prod->cur_symbol = symbol;
		printf("pos symbol cur_symbol at updateCur_symbol(): %d %d %d\n", pos, symbol, prod->cur_symbol);
    }
}

DynamicArray *insertProds() {
	int size = ARRAY_LENGTH(productions);
    DynamicArray *originalProdArray = createDynamicArray(size, true, true, PRODUCTION);
	
    for (int i = 0; i < size; i++) {
        appendCopiedOriginalProd(originalProdArray, i, PRODUCTION);
    }
	return originalProdArray;
}

DynamicArray *setUpOriginalProd() {
    DynamicArray *originalProdArary = insertProds();

    quickSort(originalProdArary, cur_symbolGetter, 0, getArrayOffset(originalProdArary), PRODUCTION);
    return originalProdArary;
}

/*
 ------  transistedProds_Item  ------
 |create  |-----------------|create  |
 |Item    |      Symbol     |Item    |
  ---------                  --------
 */
DynamicArray *setUpDupliSortUpdateProd(DynamicArray *oldProdArray, Type type) {
    if (type != PRODUCTION) error("type mismatch: setSymbol\n");
    DynamicArray *duplicatedProdsArray = cloneArray(oldProdArray, true);
    quickSort(duplicatedProdsArray, leftGetter, 0, getArrayOffset(duplicatedProdsArray), PRODUCTION);
    updateCur_Symbol(duplicatedProdsArray, PRODUCTION);
    return duplicatedProdsArray;
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


int main() {
    initializeProduction(dummy_prod);
    initializeItem(dummy_item);
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
    for (int i = 0; i <= getArrayOffset(arr); ++i) {
        int *value = (int *)retriveData(arr, i, INT);
        printf("%d ", *value);
    }
    printf("\n");

    swapElement(arr, 0, 2, INT);

    printf("Array after swapping: ");
    for (int i = 0; i <= getArrayOffset(arr); ++i) {
        int *value = (int *)retriveData(arr, i, INT);
        printf("%d ", *value);
    }
    printf("\n");

    // Test Case 3: Sorting elements in the dynamic array
    printf("Array before sorting: ");
    for (int i = 0; i <= getArrayOffset(arr); ++i) {
        int *value = (int *)retriveData(arr, i, INT);
        printf("%d ", *value);
    }
    printf("\n");

    quickSort(arr, getElementKey, 0, getArrayOffset(arr), INT);

    printf("Array after sorting: ");
    for (int i = 0; i <= getArrayOffset(arr); ++i) {
        int *value = (int *)retriveData(arr, i, INT);
        printf("%d ", *value);
    }
    printf("\n");

    // Test Case 4: Cleaning up and destroying the dynamic array
    destroyDynamicArray(arr);

    return 0;
}
/*
int main() {
    initializeProduction(dummy_prod);
    initializeItem(dummy_item);
 
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

