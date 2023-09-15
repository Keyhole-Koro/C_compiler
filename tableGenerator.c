#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamicArray.h"
#include "utilities.h"
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

int cur_symbolGetter(Production *prod) {
    return prod->cur_symbol;
}

int leftGetter(Production *prod) {
    return prod->left;
}

void duplicateProds(DynamicArray *newProdArr, DynamicArray *prodArr, Type type) {
    if (type != PRODUCTION) error("type mismatch: setSymbol\n");
    for (int i = 0; i < getNumElements(prodArr); i++) {
        Production *copied_prod = malloc(sizeof(Production));
        *copied_prod = *(Production *)getData(prodArr, i, PRODUCTION);
        append(newProdArr, copied_prod, PRODUCTION);
    }
}

int prodPartition(DynamicArray* arr, int (referentFunc)(Production *), int low, int high, Type type) {
    if (type != PRODUCTION) error("type mismatch: sortProd\n");
    Production *high_prod = (Production *)getData(arr, high, PRODUCTION);
    int pivot = referentFunc(high_prod);
    Production *j_prod;
    
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        j_prod = (Production *)getData(arr, j, PRODUCTION);
        if (referentFunc(j_prod) < pivot) {
            i++;
            swapElement(arr, i, j, PRODUCTION);
        }
    }
    swapElement(arr, i + 1, high, PRODUCTION);
    return (i + 1);
}

//sort productions according the number of symbols
void sortProd(DynamicArray *arr, int (referentFunc)(Production *), int low, int high, Type type) {
    if (type != PRODUCTION) error("type mismatch: sortProd\n");
    if (low < high) {
        int pi = prodPartition(arr, referentFunc, low, high, PRODUCTION);

        sortProd(arr, referentFunc, low, pi - 1, PRODUCTION);
        sortProd(arr, referentFunc, pi + 1, high, PRODUCTION);
    }
}

int getCurSymbol(Data* data) {
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

void removeElement(DynamicArray *arr, int i) {
    Production *prod = (Production *)getData(arr, i, PRODUCTION);
    *prod = *(Production *)empty_prod;
}


void deprioritizeProd(DynamicArray *prodArr, int deprioritizedPos) {
    removeElement(prodArr, deprioritizedPos);
    //make deprioritized element move to the end of array to reduce calculation
    swapElement(prodArr, deprioritizedPos, getOffset(prodArr), PRODUCTION);
}
//this one extract single kind of prods
void appendProdLeftIs(DynamicArray *fetchedProdArray, DynamicArray *duplicatedProdsArray, int expectedSymbol) {
    int condition = getNumElements(duplicatedProdsArray);
    for (int i = 0; i < condition; i++) {
        Production *prod = (Production *)getData(duplicatedProdsArray, i, PRODUCTION);
        if (prod->left == -1) continue;
        if (prod->left == expectedSymbol) {
            Production *copy_prod = (Production *)malloc(getDataSize(PRODUCTION));
            *copy_prod = *prod;
            append(fetchedProdArray, copy_prod, PRODUCTION);//&productions[0] assuming n and the order of elements are sync
            if (getNumElements(duplicatedProdsArray) >= 2) deprioritizeProd(duplicatedProdsArray, i);
        }
    }
}

void prioritizeSymbol(DynamicArray *fetchedSymbolArray, int symbol) {
    int pos_symbol = fetchPosition(fetchedSymbolArray, cmpSymbol, (Data *)&symbol, INT);
    printf("pos symbol: %d\n", pos_symbol);
    //swap and prioritize the recent element to avoid excess calculate
    if (pos_symbol != 0 && getNumElements(fetchedSymbolArray) > 1) swapElement(fetchedSymbolArray, pos_symbol, getOffset(fetchedSymbolArray), INT);
}
//this one extracts multiple kind of prods
void extractNessesaryProds(DynamicArray *fetchedProdArray, DynamicArray *duplicatedProdsArray) {
    DynamicArray *fetchedSymbolArray = createDynamicArray(10, INT);
    int offset = getNumElements(fetchedProdArray);
    for (int i = 0; i < offset; i++) {

        Production *prod = (Production *)getData(fetchedProdArray, i, PRODUCTION);
        if (prod->left == -1) continue;
                        
        int cur_symbol = prod->cur_symbol;

        if (!isNonTerminal(cur_symbol) || isSymbolOverlap(fetchedSymbolArray, cur_symbol, INT)) continue;

        int copied_symbol = (int)malloc(sizeof(int));
        copied_symbol = cur_symbol;
        append(fetchedSymbolArray, &copied_symbol, INT);
        
        prioritizeSymbol(fetchedSymbolArray, cur_symbol);
        
        appendProdLeftIs(fetchedProdArray, duplicatedProdsArray, cur_symbol);
        
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
            if (dup_prod->key == fet_prod->key) deprioritizeProd(duplicatedProdsArray, j);
        }
    }
}

void setUpDupliProd(DynamicArray *duplicatedProdsArray, DynamicArray *fetchedProdArray) {
    insertProds(duplicatedProdsArray, PRODUCTION);

    sortProd(duplicatedProdsArray, cur_symbolGetter, 0, getOffset(duplicatedProdsArray), PRODUCTION);
    
    eliminateOverlap(duplicatedProdsArray, fetchedProdArray);
}

void updateCur_Symbol(DynamicArray *prodArr, Type type) {
    if (type != PRODUCTION) error("type mismatch: setSymbol\n");
    for (int i = 0; i < getNumElements(prodArr); i++) {
        Production *prod = (Production *)getData(prodArr, i, PRODUCTION);
        //look ahead and increament
        int pos = prod->readPosition++;
        //update cur_symbol
        int symbol = prod->right[++pos];
        prod->cur_symbol = symbol;
    }
}

/*
  --------  prodTransitter   --------
 |create  |-----------------|create  |
 |Item    |      Symbol     |Item    |
  ---------                  --------
 */
void setUpTransisterdProd(DynamicArray *newProdArr, DynamicArray *oldProdArray, Type type) {
    if (type != PRODUCTION) error("type mismatch: setSymbol\n");
    duplicateProds(newProdArr, oldProdArray, PRODUCTION);
    sortProd(newProdArr, leftGetter, 0, getOffset(newProdArr), PRODUCTION);
    incrementPosition(newProdArr, PRODUCTION);
    updateCur_Symbol(newProdArr, PRODUCTION);
}

DynamicArray *extractProd(DynamicArray *originalProdArr, DynamicArray *fetchedProdArray, int expectedSymbol) {
    DynamicArray *extractedProdArr = createDynamicArray(getNumElements(fetchedProdArray)/2, PRODUCTION);
    for (j = beginning_point; j < getNumElements(originalProdArr); j++){
        Production *prod = (Production *)getData(originalProdArr, j, PRODUCTION);
        if (prod->left != expectedSymbol) break;
        append(extractedProdArr, prod, PRODUCTION);
    }
    return extractProdArr;
}
//combine prodTransitter and createItem partially later
void prodTransitter(DynamicArray *itemArray, DynamicArray *oldProdArray, DynamicArray *fetchedSymbolArray) {
    DynamicArray *newProdArr = createDynamicArray(getNumElements(fetchedProdArray), PRODUCTION);
    setUpTransisterdProd(newProdArr, oldProdArray, PRODUCTION);
    
    int beginning_point = 0;
    int j = 0;
    DynamicArray *newItems = createDynamicArray(getNumElements(fetchedSymbolArray, ITEM);
    for (int i = 0; i < getNumElements(fetchedSymbolArray; i++)) {
        int symbol = (int)*getData(fetchedSymbolArray, i, INT);
        DynamicArray *extractedProdArr = extractProd(newProdArr, fetchedProdArray, symbol);

        if (isEndProd(extractedProdArr)) continue;//not yet
        
        Item *new_item = createItem(itemArray, extractedProdArr, symbol);
        append(newItems, (Data *)new_item, ITEM);
        beginning_point = --j;
    }
    Item *previous_item = (Item *)getData(itemArray, ,ITEM);
    
    free(newProdArr);
}

Data *fetchItem() {
    int pos_item = fetchPosition();// not yet
}

//the copying prod takes much time
Item *createItem(DynamicArray *itemArray, DynamicArray *fetchedProdArray, int expectedSymbol) {
    Data *certainExistingItem = fetchItem();//not yet
    
    if (duplicatedItem != NULL) {
        itemArray = certainExistingItem;
    } else {
        int size = ARRAY_LENGTH(productions) * sizeof(Production);
        DynamicArray *duplicatedProdsArray = createDynamicArray(size+1, PRODUCTION);
        
        setUpDupliProd(duplicatedProdsArray, fetchedProdArray);
        
        extractNessesaryProds(fetchedProdArray, duplicatedProdsArray);
        printf("---------\n");
        for (int i = 0; i < getNumElements(fetchedProdArray); i++) {
            Production *prod = (Production *)getData(fetchedProdArray, i, PRODUCTION);
            printf("key cur_symbol %d %d\n", prod->key, prod->cur_symbol);
        }
        prodTransitter(itemArray, fetchedProdArray, fetchedSymbolArray);
    }
    //make insertItem later
    Item *new_item = (Item *)malloc(sizeof(Item));
    new_item->stateId = getNumElements(itemArray);
    new_item->transitionedSymbol = expectedSymbol;
    new_item->Productions = (Production **)&fetchedProdArray;
    new_item->transitionDestinations = ;//not yet
        
    append(itemArray, new_item, ITEM);
    
    free(duplicatedProdsArray);
    
    return new_item;//tmp
}

int main() {
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
