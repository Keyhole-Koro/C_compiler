#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamicArray.h"

/*
 deprioritize()
 fetchedProdArray:
 duplicatedProdsArray: 0 1 2 3 4 5
 originalProd: 0 1 2 3 4 5
 ----
 fetchedProdArray: 2
 duplicatedProdsArray: 0 1 5 3 4 N
 originalProd: 0 1 2 3 4 5
 */

/*
 swapElement()
 fetchedSymbols: E T F
 F apperd
 ----
 fetchedSymbols: F T E
 */

enum {
    NUM = 1,      //number
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
    
    EXPR = 256, // E
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
    {8, FACTOR, {NUM}, 0, NUM},
};

Production empty_prod_instance = {-1, -1, {-1}, -1, -1};
Production *empty_prod = &empty_prod_instance;

bool isNonTerminal(int symbol) {
    if (symbol >= 256) return true;
    return false;
}

int partition(DynamicArray* arr, int low, int high) {
    Production *high_prod = (Production *)getData(arr, high, PRODUCTION);
    int pivot = high_prod->cur_symbol;
    Production *j_prod;
    
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        j_prod = (Production *)getData(arr, j, PRODUCTION);
        if (j_prod->cur_symbol < pivot) {
            i++;
            swapElement(arr, i, j, PRODUCTION);
        }
    }
    swapElement(arr, i + 1, high, PRODUCTION);
    return (i + 1);
}

//sort productions according the number of symbols
void sortProd(DynamicArray *arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        sortProd(arr, low, pi - 1);
        sortProd(arr, pi + 1, high);
    }
}

int getCurSymbol(Data* data) {
    Production *prod = (Production *)(data);
    return prod->cur_symbol;
}

void setSymbol(DynamicArray *prodArr, Type type) {
    if (type != PRODUCTION) error("type mismatch: setSymbol\n");
    for (int i = 0; i < getOffset(prodArr); i++) {
        Production *prod = (Production *)getData(prodArr, i, PRODUCTION);
        int pos = prod->readPosition;
        int symbol = prod->right[pos];
        prod->cur_symbol = symbol;
    }
}

void incrementPosition(DynamicArray *prodArr, Type type) {
    if (type != PRODUCTION) error("type mismatch: incrementPosition\n");
    Production *prod;
    for (int i = 0; i < getOffset(prodArr); i++) {
        prod = (Production *)getData(prodArr, i, PRODUCTION);
        prod->readPosition++;
    }
}

bool isOverlap(DynamicArray *symbolArr, int symbol, Type type) {
    if (type != INT) error("type mismatch: isOverlap\n");
    Production *prod;
    for (int i = 0; i < getOffset(symbolArr); i++) {
        int *existing_symbol = (int *)getData(symbolArr, i, INT);
        if (*existing_symbol == symbol) return true;
    }
    return false;
}

void removeElement(DynamicArray *arr, int i) {
    Production *prod = (Production *)getData(arr, i, PRODUCTION);
    *prod = *(Production *)empty_prod;
}


void deprioritizeProd(DynamicArray *prodArr, int deprioritizedPos) {
    removeElement(prodArr, deprioritizedPos);
    //make deprioritized element move to the end of array
    swapElement(prodArr, deprioritizedPos, getOffset(prodArr), PRODUCTION);
}
//this one extract single kind of prods
void appendProdLeftIs(DynamicArray *fetchedProdArray, DynamicArray *duplicatedProdsArray, int expectedSymbol) {
    int condition = getNumElements(duplicatedProdsArray);
    for (int i = 0; i < condition; i++) {
        Production *prod = (Production *)getData(duplicatedProdsArray, i, PRODUCTION);
        if (prod->left == -1) continue;
        if (prod->left == expectedSymbol) {
            append(fetchedProdArray, &productions[i], PRODUCTION);
            if (getNumElements(duplicatedProdsArray) >= 2) deprioritizeProd(duplicatedProdsArray, i);
        }
    }
}

bool cmpSymbol(Data *data, Data *expectedValue) {
    int *symbol = (int *)data;
    int *compedValue = (int *)expectedValue;
    return *symbol == *compedValue;
}

//this one extracts multiple kind of prods
void extractNessesaryProds(DynamicArray *fetchedProdArray, DynamicArray *duplicatedProdsArray) {
    DynamicArray *fetchedSymbolArray = createDynamicArray(10, INT);
    int offset = getNumElements(fetchedProdArray);
    for (int i = 0; i < offset; i++) {
        printf("i: %d\n", i);
        Production *prod = (Production *)getData(fetchedProdArray, i, PRODUCTION);
        if (prod->left == -1) continue;
                        
        int symbol = prod->cur_symbol;
        printf("num of fetched array: %d\n", getOffset(fetchedProdArray));
        printf("extract num: %d\n", prod->n);
        printf("extract symbol: %d\n", symbol);
        if (!isNonTerminal(symbol)) continue;
        // || isOverlap(fetchedSymbolArray, symbol, INT)
        //printf("i: %d\n", i);
        printf("fetched symbol: %d\n", symbol);
        append(fetchedSymbolArray, &symbol, INT);
        int pos_symbol = fetchPosition(fetchedSymbolArray, cmpSymbol, (Data *)&symbol, INT);
                
        //swap and prioritize the recent element to avoid excess calculate
        if (pos_symbol != 0 && getNumElements(fetchedSymbolArray) > 1) swapElement(fetchedSymbolArray, pos_symbol, getOffset(fetchedSymbolArray), INT);
        
        appendProdLeftIs(fetchedProdArray, duplicatedProdsArray, symbol);
        
        offset = getNumElements(fetchedProdArray);
    }
}

void insertProds(DynamicArray *arr, Type type) {
    for (int i = 0; i < ARRAY_LENGTH(productions); i++) {
        Production *copied_prod = malloc(sizeof(Production));
        *copied_prod = productions[i];
        append(arr, copied_prod, PRODUCTION);
    }
}

void eliminateOverlap(DynamicArray *duplicatedProdsArray, DynamicArray *fetchedProdArray) {
    for (int i = 0; i < getNumElements(fetchedProdArray); i++) {
        Production *fet_prod = (Production *)getData(fetchedProdArray, i, PRODUCTION);
        for (int j = 0; j < getNumElements(duplicatedProdsArray); j++) {
            Production *dup_prod = (Production *)getData(duplicatedProdsArray, j, PRODUCTION);
            if (dup_prod->n == fet_prod->n) deprioritizeProd(duplicatedProdsArray, j);
        }
    }
}

void setUpDupliProd(DynamicArray *duplicatedProdsArray, DynamicArray *fetchedProdArray) {
    insertProds(duplicatedProdsArray, PRODUCTION);

    sortProd(duplicatedProdsArray, 0, getOffset(duplicatedProdsArray));
    
    eliminateOverlap(duplicatedProdsArray, fetchedProdArray);
}
//the copying prod takes much time
Item *createItem(DynamicArray *itemArray, DynamicArray *fetchedProdArray, int readSymbol){
    int size = ARRAY_LENGTH(productions) * sizeof(Production);
    DynamicArray *duplicatedProdsArray = createDynamicArray(size+1, PRODUCTION);
    
    setUpDupliProd(duplicatedProdsArray, fetchedProdArray);
    
    extractNessesaryProds(fetchedProdArray, duplicatedProdsArray);
    printf("---------\n");
    for (int i = 0; i < getOffset(fetchedProdArray); i++) {
        Production *prod = (Production *)getData(fetchedProdArray, i, PRODUCTION);
        printf("n cur_symbol %d %d\n", prod->n, prod->cur_symbol);
    }

    
    Item *new_item = (Item *)malloc(sizeof(Item));
    new_item->stateId = numItems;
    new_item->transitionedSymbol = readSymbol;
    new_item->Productions = (Production *)fetchedProdArray;
    new_item->transitionDestinations = (Item *)itemArray;
        
    append(itemArray, new_item, ITEM);
    
    free(duplicatedProdsArray);
    
    return new_item;//tmp
}

int main() {
    DynamicArray *itemArray = createDynamicArray(10, ITEM);
    
    DynamicArray *fetchedProdArray = createDynamicArray(10, PRODUCTION);
    append(fetchedProdArray, &productions[0], PRODUCTION);

    Item *first_item = createItem(itemArray, fetchedProdArray, EXPR);//temporary
    
    
}

//createFirst()
//getFirst()
//createFollow()
//getFollow()
//createState()
//sortRows(
//sortColumns()
//readItem()
