#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamicArray.h"
#include "utilities.h"

#include <unistd.h>


Item *createItem(DynamicArray *itemArray, DynamicArray *fetchedProdArray, int expectedSymbol);

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
    {0, EXPR, {EXPR, END, -1}, 0, EXPR},
    {1, EXPR, {EXPR, ADD, TERM, -1}, 0, EXPR},
    {2, EXPR, {EXPR, SUB, TERM, -1}, 0, EXPR},
    {3, EXPR, {TERM, -1}, 0, TERM},
    {4, TERM, {TERM, MUL, FACTOR, -1}, 0, TERM},
    {5, TERM, {TERM, DIV, FACTOR, -1}, 0, TERM},
    {6, TERM, {FACTOR, -1}, 0, FACTOR},
    {7, FACTOR, {L_PARENTHESES, EXPR, R_PARENTHESES, -1}, 0, L_PARENTHESES},
    {8, FACTOR, {NUM, -1}, 0, NUM},
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

int prodPartition(DynamicArray *arr, int (referentFunc)(Production *), int low, int high, Type type) {
    printf("prodPartition\n");
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

int pos_validProdFromTail(DynamicArray *prodArr, Type type) {
    if (type != PRODUCTION) error("type mismatch: pos_validProdFromTail\n");
    int pos = getOffset(prodArr);
    for (int i = getOffset(prodArr); i > 0; i--) {
        Production *prod = (Production *)getData(prodArr, i, PRODUCTION);
        pos--;
    }
    return pos;
}


//this one extract single kind of prods
void appendProdLeftIs(DynamicArray *fetchedProdArray, DynamicArray *duplicatedProdsArray, int expectedSymbol) {
    for (int i = 0; i < getNumElements(duplicatedProdsArray); i++) {
        Production *prod = (Production *)getData(duplicatedProdsArray, i, PRODUCTION);
        if (prod->left == expectedSymbol) {
            Production *copy_prod = (Production *)malloc(getDataSize(PRODUCTION));
            *copy_prod = *prod;
            append(fetchedProdArray, copy_prod, PRODUCTION);//&productions[0] assuming n and the order of elements are sync
            if (getNumElements(duplicatedProdsArray) >= 2) deprioritizeArray(duplicatedProdsArray, i, PRODUCTION);
        }
    }
}

void prioritizeSymbol(DynamicArray *fetchedSymbolArray, int symbol) {
    printf("prioritizeSymbol\n");
    int pos_symbol = fetchPosition(fetchedSymbolArray, cmpSymbol, (Data *)&symbol, INT);
    //swap and prioritize the recent element to avoid excess calculate
    if (pos_symbol != 0 && getNumElements(fetchedSymbolArray) > 1) swapWithLastElement(fetchedSymbolArray, pos_symbol, INT);
}

//this one extracts multiple kind of prods
DynamicArray *gatherNessesaryProds(DynamicArray *fetchedProdArray, DynamicArray *duplicatedProdsArray, DynamicArray *fetchedSymbolArray) {
    int num_elements = getNumElements(fetchedProdArray);
    for (int i = 0; i < num_elements; i++) {
        printf("num_elements: %d\n", num_elements);

        Production *prod = (Production *)getData(fetchedProdArray, i, PRODUCTION);
                        
        int cur_symbol = prod->cur_symbol;

        if (!isNonTerminal(cur_symbol) || isSymbolOverlap(fetchedSymbolArray, cur_symbol, INT)) continue;

        int *copied_symbol = (int *)malloc(sizeof(int *));
        *copied_symbol = cur_symbol;
        
        printf("prod key: %d\n", prod->key);
        printf("prod cur_symbol: %d\n", prod->cur_symbol);
        
        append(fetchedSymbolArray, copied_symbol, INT);
        
        prioritizeSymbol(fetchedSymbolArray, cur_symbol);
        
        appendProdLeftIs(fetchedProdArray, duplicatedProdsArray, cur_symbol);
        
        num_elements = getNumElements(fetchedProdArray);
        
    }
    return fetchedProdArray;
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
            if (dup_prod->key == fet_prod->key) deprioritizeArray(duplicatedProdsArray, j, PRODUCTION);
        }
    }
}

DynamicArray *setUpDupliProd(DynamicArray *fetchedProdArray) {
    int size = ARRAY_LENGTH(productions) * sizeof(Production);
    DynamicArray *duplicatedProdsArray = createDynamicArray(size + 1, true, PRODUCTION);
    
    insertProds(duplicatedProdsArray, PRODUCTION);

    sortProd(duplicatedProdsArray, cur_symbolGetter, 0, getOffset(duplicatedProdsArray), PRODUCTION);
    
    eliminateOverlap(duplicatedProdsArray, fetchedProdArray);
    
    return duplicatedProdsArray;
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
DynamicArray *setUpTransisterdProd(DynamicArray *oldProdArray, Type type) {
    if (type != PRODUCTION) error("type mismatch: setSymbol\n");
    DynamicArray *duplicatedProdsArray = duplicateArray(oldProdArray, true);
    sortProd(duplicatedProdsArray, leftGetter, 0, getOffset(duplicatedProdsArray), PRODUCTION);
    updateCur_Symbol(duplicatedProdsArray, PRODUCTION);
    return duplicatedProdsArray;
}

//make this optimize later particulary using deprioritizer
bool isEndProd(DynamicArray *prodArr) {
    if (getNumElements(prodArr) != 1) return false;
    Production *singleProd = (Production *)getData(prodArr, 0, PRODUCTION);
    int pos_read = singleProd->readPosition;
    int num_right = 0;
    for (int i = 0; i < sizeof(singleProd->right); i++) {
        num_right++;
    }
    //which means the current symbol is the end of right side
    if (pos_read == num_right) return true;
    return false;
}

//you may be able to find faster way other than deprioritizer
DynamicArray *extractProd(DynamicArray *copiedProdArray, int expectedSymbol) {
    DynamicArray *extractedProdArr = createDynamicArray(getNumElements(copiedProdArray), false, PRODUCTION);
    for (int i = 0; i < getNumElements(copiedProdArray); i++){
        Production *copiedProd = (Production *)getData(copiedProdArray, i, PRODUCTION);
        int sym = copiedProd->cur_symbol;
        
        if (sym != expectedSymbol) continue;
        printf("sym: %d\n", sym);
        //copied one will be deprioritized thats why you use the original one
        Production *new_copiedProd = (Production *)malloc(sizeof(copiedProd));
        *new_copiedProd = *copiedProd;
        append(extractedProdArr, new_copiedProd, PRODUCTION);
        deprioritizeArray(copiedProdArray, i, PRODUCTION);
        }
    return extractedProdArr;
}


//make this optimize later particulary using deprioritizer
void prodTransitter(DynamicArray *itemArray, DynamicArray *fetchedProdArray, DynamicArray *symbolArray) {
    //duplicated
    DynamicArray *duplicatedProdArray = setUpTransisterdProd(fetchedProdArray, PRODUCTION);
    
    DynamicArray *newItems = createDynamicArray(getNumElements(symbolArray), false, ITEM);
    
    //this part is supposed to be done before createItem() below
    int cur_latest_num_item = getOffset(itemArray);

    for (int i = 0; i < getNumElements(duplicatedProdArray); i++) {
        Production *prod = (Production *)getData(duplicatedProdArray, i, PRODUCTION);
        int *symbol = (int *)&(prod->cur_symbol);
        
        if (*symbol == -1) continue;
        printf("expected symbol: %d\n", *symbol);
        DynamicArray *extractedProdArr = extractProd(duplicatedProdArray, *symbol);
        
        if (getNumElements(extractedProdArr) == 0) continue;
        
        if (isEndProd(extractedProdArr)) continue;
        printf("num of extracted prods: %d\n", getNumElements(extractedProdArr));
        Item *part_new_item = createItem(itemArray, extractedProdArr, *symbol);
        append(newItems, part_new_item, ITEM);
    }

    if (cur_latest_num_item > 0) {
        Item *cur_latest_item = (Item *)getData(itemArray, cur_latest_num_item,ITEM);
        cur_latest_item->transitionDestinations = (Item *)newItems;
    }
    //destroyDynamicArray(newProdArr);
}

//this take much time
bool isExsistingItem(Item *referentItem, Production *prod) {
    int value_hashed_key = referentItem->hashed_keys;
    Production *referentProd = getProd_Item(referentItem, ITEM);
    if (calculateSetHash((DynamicArray *)referentProd, getKey, PRODUCTION) == value_hashed_key) return true;
    
    return false;
}

Item *fetchItem(DynamicArray *itemArray, DynamicArray *prodArray, int expectedSymbol) {
    if (getNumElements(itemArray) == 0) return NULL;
    Item *latest_arr = (Item *)getData(itemArray, getOffset(itemArray), ITEM);
    DynamicArray *pos_items = fetchMultiPositions(itemArray, cmpTransitionedSymbol, (Data *)&expectedSymbol, ITEM);
    for (int i = 0; i < getNumElements(pos_items); i++) {
        Item *item = (Item *)getData(itemArray, i, ITEM);//sus *pos - 1
        if (latest_arr->transitionedSymbol != expectedSymbol) continue;
        if (isExsistingItem(item, (Production *)prodArray)) return item;
    }
    
    destroyDynamicArray(pos_items);
    return NULL;
}

Item *setItem(DynamicArray *itemArray, int transitionedSymbol, DynamicArray *fetchedProdArray) {
    Item *new_item = (Item *)malloc(sizeof(Item));
    new_item->stateId = getNumElements(itemArray);
    new_item->transitionedSymbol = transitionedSymbol;
    new_item->Productions = (Production *)fetchedProdArray;
    new_item->hashed_keys = calculateSetHash(fetchedProdArray, getKey, PRODUCTION);
    append(itemArray, new_item, ITEM);
    return (Item *)getData(itemArray, getOffset(itemArray), ITEM);//it can be able to be replaced to new_item
}

//the copying prod takes much time
Item *createItem(DynamicArray *itemArray, DynamicArray *fetchedProdArray, int expectedSymbol) {
    printf("num of item: %d\n", getNumElements(itemArray));
    printf("---------------\ncreateItem: %d\n", getNumElements(fetchedProdArray));
    for (int i = 0; i < getNumElements(fetchedProdArray); i++) {
        Production *prod = (Production *)getData(fetchedProdArray, i, PRODUCTION);
        printf("key, cur_symbol: %d %d\n", prod->key, prod->cur_symbol);
    }
    Item *existingItem = fetchItem(itemArray, fetchedProdArray, expectedSymbol);
    if (existingItem != NULL) {
        return existingItem;
    } else {
        DynamicArray *fetchedSymbolArray = createDynamicArray(10, true, INT);

        DynamicArray *duplicatedProdsArray = setUpDupliProd(fetchedProdArray);
        
        //dont have to be returned you can use fetchedProdArray in the same way as gatheredProdArray
        DynamicArray *gatheredProdArray = gatherNessesaryProds(fetchedProdArray, duplicatedProdsArray, fetchedSymbolArray);
        printf("---------\n");
        for (int i = 0; i < getNumElements(fetchedProdArray); i++) {
            Production *prod = (Production *)getData(gatheredProdArray, i, PRODUCTION);
            printf("key cur_symbol %d %d\n", prod->key, prod->cur_symbol);
        }
        Item *cur_latest_item = setItem(itemArray, expectedSymbol, gatheredProdArray);

        prodTransitter(itemArray, gatheredProdArray, fetchedSymbolArray);
        
        return cur_latest_item;
    }
}

int main() {
    DynamicArray *itemArray = createDynamicArray(10, false, ITEM);
    DynamicArray *fetchedProdArray = createDynamicArray(10, false, PRODUCTION);
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

