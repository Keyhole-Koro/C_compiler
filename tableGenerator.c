#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamicArray.h"

enum {
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

Item *listItems[100];
int numItems = 0;

//{EXPR, ADD, TERM}
//   0    1     2
Production productions[] = {
    {0, EXPR, {EXPR, END}, 0, EXPR},
    {1, EXPR, {EXPR, ADD, TERM}, 0, EXPR},
    {2, EXPR, {EXPR, SUB, TERM}, 0, EXPR},
    {3, EXPR, {TERM}, 0, TERM},
    {4, TERM, {TERM, MUL, FACTOR}, 0, TERM},
    {5, TERM, {TERM, DIV, FACTOR}, 0, TERM},
    {6, TERM, {FACTOR}, 0, FACTOR},
    {7, FACTOR, {L_PARENTHESES, EXPR, R_PARENTHESES}, 0, L_PARENTHESES},
    {8, FACTOR, {NUM}, 0, NUM}
};

int partition(DynamicArray* arr, int low, int high) {
    int pivot = arr[high]->cur_symbol;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j]->cur_symbol < pivot) {
            i++;
            swapElements(arr[i], arr[j]);
        }
    }
    swapElements(arr[i + 1], arr[high]);
    return (i + 1);
}

//sort productions according the number of symbols
void sortProd(DynamicArray* arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        sortProd(arr, low, pi - 1);
        sortProd(arr, pi + 1, high);
    }
}

int getCurSymbol(Data* prod) {
    return prod->cur_symbol;
}
DynamicArray *insertProds(DynamicArray *arr, Type type) {
    arr->data = malloc(initialCapacity * getDataSize(type));
    if (arr->data == NULL) error("Memory allocation failed\n");
    
    for (int i = 0; i < sizeof(productions); i++) {
        append(arr, &productions[i], PRODUCTION);
    }
    return arr;
}

void setSymbol(DynamicArray *prodArr, Type type) {
    if (type != PRODUCTION) error("type mismatch\n");
    for (int i = 0; i < getOffset(prodArr); i++) {
        int pos = prodArr[i]->readPosition;
        int symbol = prodArr[i]->left[pos];
        prodArr[i]->cur_symbol = symbol;
    }
}

void incrementPosition(DynamicArray *prodArr, Type type) {
    if (type != PRODUCTION) error("type mismatch\n");
    for (int i = 0; i < getOffset(prodArr); i++) {
        prodArr[i]->readPosition ++;
    }
}

bool isOverlap(DynamicArray *symbolArr, int symbol, Type type) {
    if (type != INT) error("type mismatch\n");
    for (int i = 0; i < getOffset(symbolArr); i++) {
        if (*symbolArr[i] == symbol) return false;
    }
    return true;
}

void removeElement(DynamicArray *arr, int i) {
    arr[i] = (Production){NULL, {NULL}, NULL};
}

Data *copyProdbutPointer(int num_prod) {
    Data *duplicated_prod = (Data *)malloc(sizeof(Production));
    *duplicated_prod = productions[num_prod];
    return duplicated_prod;
}
/*
 fetchedProdArray:
 duplicatedProdsArray: 0 1 2 3 4 5
 originalProd: 0 1 2 3 4 5
 ----
 fetchedProdArray: 2
 duplicatedProdsArray: 0 1 5 3 4 N
 originalProd: 0 1 2 3 4 5
 */
//this one extract single kind of prods
void appendProdLeftIs(DynamicArray *fetchedProdArray, DynamicArray *duplicatedProdsArray, DynamicArray *fetchedSymbols, int expectedSymbol) {
    int condition = getOffset(duplicatedProdsArray);
    for (int i = 0; i > condition; i++) {
        if (duplicatedProdsArray[i]->left == expectedSymbol) {
            append(fetchedProdArray, &productions[i], PRODUCTION);
            remove(duplicatedProdsArray, i);
        }
    }
}

/*
 fetchedSymbols: E T F
 F apperd
 ----
 fetchedSymbols: F T E
 */
bool cmpSymbol(Data *data, Data *expectedValue) {
    int *symbol = (int *)data;
    int *compedValue = (int *)expectedValue;
    return *symbol == *compedValue;
}

//this one extracts multiple kind of prods
void *extractNessesaryProds(DynamicArray *fetchedProdArray, Production *duplicatedProdsArray) {
    DynamicArray *fetchedSymbolArray = createDynamicArray(5, int);
    for (int i = 0; i < getOffset(fetchedProdArray) + 1; i++) {
        if (fetchedProdArray[i].left == NULL) continue;
                        
        int symbol = fetchedProdArray[i]->cur_symbol;
        
        if (!isNonTerminal(symbol) || isOverlap(fechedSymbolArray, symbol, INT) continue;
        
        append(fetchedSymbols, &symbol, INT);
        
        int pos_symbol = fetchPosition(fetchedSymbols, cmpSymbol, (Data *)&symbol, INT)
        
        if (getOffset(fetchedSymbols) >= 2) swapElements(fetchedSymbols, pos_symbol, getOffset(fetchedSymbols));
        
        appendProdLeftIs(fetchedProdArray, duplicatedProdsArray, symbol, fetchedSymbols, getOffset(duplicatedProdsArray));
    }
}
//the copying prod takes much time
Item *createItem(DynamicArray *itemArray, DynamicArray *fetchedProdArray, int readSymbol){
    int size = ARRAY_LENGTH(Productions) * sizeof(Production);
    DynamicArray *duplicatedProdsArray = createDynamicArray(size, PRODUCTION);
    insertProds(duplicatedProdsArray, PRODUCTION);
    sortProd(duplicatedProdsArray, 0, getOffset(duplicatedProdsArray) - 1);
    
    extractNessesaryProds(fetchedProdArray, duplicatedProdsArray);
    
    //have yet to implement dot
    int *symbols = getSymbol(prodArray);
    for (int i = 0; i < ARRAY_LENGTH(symbols); i++) {
        createItem();
    }
    
    Item *new_item = (Item *)malloc(sizeof(Item));
    new_item->stateId = numItems;
    new_item->transitionedSymbol = readSymbol;
    new_item->Productions = prodArray;
    new_item->transitionDestinations = itemArray;
        
    append(itemArray, new_item, ITEM);
    
    free(duplicatedProd);
}

int main() {
    DynamicArray *itemArray = createDynamicArray(10, ITEM);
    
    DynamicArray *fetchedProdArray = createDybamicArray(10, PRODUCTION);
    append(fetchedProdArray, &productions[0], PRODUCTION);

    Item *first_item = createItem(itemArray, fetchedProdArray, EXPR);//temporary
    
    
}

int main() {
    DynamicArray *intArray = createDynamicArray(10, INT);

    int intValue = 1000;
    int intValue2 = 2000;
    int intValue3 = 3000;
    append(intArray, &intValue, INT);
    append(intArray, &intValue2, INT);

    int *fetchedInt = (int *)getData(intArray, 1, INT);
    printf("Fetched Integer: %d\n", *fetchedInt);

    int pos = fetchPosition(intArray, cmpStateId, (Data *)&intValue3, INT);
    printf("Position: %d\n", pos);

    DynamicArray *itemArray = createDynamicArray(3, ITEM);

    Item itemValue;
    itemValue.stateId = 20;
    append(itemArray, &itemValue, ITEM);
    Item *fetchedItem = (Item *)getData(itemArray, 0, ITEM);
    printf("Fetched Item: stateId=%d\n", fetchedItem->stateId);

    destroyDynamicArray(intArray);
    destroyDynamicArray(itemArray);

    return 0;
}

//createFirst()
//getFirst()
//createFollow()
//getFollow()
//createState()
//sortRows(
//sortColumns()
//readItem()
