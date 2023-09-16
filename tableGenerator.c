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
        if (prod->left == -1) break;
        pos--;
    }
    return pos;
}

//please ensure the producions are copied one but the original one
void deprioritizeProd(DynamicArray *prodArr, int pos_deprioritized) {
    removeElement(prodArr, pos_deprioritized, PRODUCTION);
    //make deprioritized element move to the end of array to reduce calculation
    int pos = pos_validProdFromTail(prodArr, PRODUCTION);
    swapElement(prodArr, pos_deprioritized, pos, PRODUCTION);
}
//this one extract single kind of prods
void appendProdLeftIs(DynamicArray *fetchedProdArray, DynamicArray *duplicatedProdsArray, int expectedSymbol) {
    for (int i = 0; i < getNumElements(duplicatedProdsArray); i++) {
        Production *prod = (Production *)getData(duplicatedProdsArray, i, PRODUCTION);
        if (prod->left == -1) continue;//modified continue to break
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
    //swap and prioritize the recent element to avoid excess calculate
    if (pos_symbol != 0 && getNumElements(fetchedSymbolArray) > 1) swapElement(fetchedSymbolArray, pos_symbol, getOffset(fetchedSymbolArray), INT);
}

//this one extracts multiple kind of prods
DynamicArray *gatherNessesaryProds(DynamicArray *fetchedProdArray, DynamicArray *duplicatedProdsArray, DynamicArray *fetchedSymbolArray) {
    int num_elements = getNumElements(fetchedProdArray);
    for (int i = 0; i < num_elements; i++) {
        printf("num_elements: %d\n", num_elements);

        Production *prod = (Production *)getData(fetchedProdArray, i, PRODUCTION);
        if (prod->left == -1) continue;
                        
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
    updateCur_Symbol(newProdArr, PRODUCTION);
}

//make this optimize later particulary using deprioritizer
bool isEndProd(DynamicArray *prodArr) {
    if (getNumElements(prodArr) != 1) return false;
    Production *singleProd = (Production *)getData(prodArr, 0, PRODUCTION);
    int pos_read = singleProd->readPosition;
    int num_right = 0;
    for (int i = 0; i < sizeof(singleProd->right); i++) {
        if (singleProd->right[i] == -1) break;
        num_right++;
    }
    //which means the current symbol is the end of right side
    if (pos_read == num_right) return true;
    return false;
}

//you may be able to find faster way other than deprioritizer
DynamicArray *extractProd(DynamicArray *copiedProdArray, int expectedSymbol) {
    DynamicArray *extractedProdArr = createDynamicArray(getNumElements(copiedProdArray), PRODUCTION);
    for (int i = 0; i < getNumElements(copiedProdArray); i++){
        Production *copiedProd = (Production *)getData(copiedProdArray, i, PRODUCTION);
        int sym = copiedProd->cur_symbol;
        printf("sym: %d\n", sym);
        if (sym == -1) continue;//can be replaced to break
        if (sym != expectedSymbol) continue;
        
        //copied one will be deprioritized thats why you use the original one
        Production *new_copiedProd = (Production *)malloc(sizeof(copiedProd));
        *new_copiedProd = *copiedProd;
        append(extractedProdArr, new_copiedProd, PRODUCTION);
        deprioritizeProd(copiedProdArray, i);
        }
    return extractedProdArr;
}


//make this optimize later particulary using deprioritizer
void prodTransitter(DynamicArray *itemArray, DynamicArray *fetchedProdArray, DynamicArray *symbolArray) {
    DynamicArray *copiedProdArr = createDynamicArray(getNumElements(fetchedProdArray), PRODUCTION);
    //duplicated
    setUpTransisterdProd(copiedProdArr, fetchedProdArray, PRODUCTION);
    
    DynamicArray *newItems = createDynamicArray(getNumElements(symbolArray), ITEM);
    
    //this part is supposed to be done before createItem() below
    int cur_latest_num_item = getOffset(itemArray);

    for (int i = 0; i < getNumElements(copiedProdArr); i++) {
        Production *prod = (Production *)getData(copiedProdArr, i, PRODUCTION);
        int *symbol = (int *)&(prod->cur_symbol);
        
        if (*symbol == -1) continue;
        printf("expected symbol: %d\n", *symbol);
        DynamicArray *extractedProdArr = extractProd(copiedProdArr, *symbol);
        
        if (getNumElements(extractedProdArr) == 0) continue;
        
        if (isEndProd(extractedProdArr)) continue;
        printf("num of extracted prods: %d\n", getNumElements(extractedProdArr));
        Item *new_item = createItem(itemArray, extractedProdArr, *symbol);
        append(newItems, new_item, ITEM);
    }

    if (cur_latest_num_item > 0) {
        Item *cur_latest_item = (Item *)getData(itemArray, cur_latest_num_item,ITEM);
        cur_latest_item->transitionDestinations = (Item *)newItems;
    }
    //destroyDynamicArray(newProdArr);
}

//add deprioritizer
bool isThereSameProd(Production *prod, DynamicArray *prodArray) {
    for (int i = 0; i < getNumElements(prodArray); i++) {
        Production *prod_arr = (Production *)getData(prodArray, i, PRODUCTION);
        int key = prod_arr->key;
        if (key == prod->key) return true;
    }
    return false;
}

//this take much time
bool isExsistingItem(Item *item, DynamicArray *prodArray) {
    int num_prodArray = getNumElements(prodArray);
    for (int i = 0; i < num_prodArray; i++) {
        Production *prod_item = item->Productions;
        if (getNumElements((DynamicArray *)prod_item) != getNumElements(prodArray)) continue;
        
        Production *prod = (Production *)getData((DynamicArray *)prod_item, i, PRODUCTION);
        
        if (!isThereSameProd(prod, prodArray)) return false;
    }
    return true;
}

Item *fetchItem(DynamicArray *itemArray, DynamicArray *prodArray, int expectedSymbol) {
    if (getNumElements(itemArray) == 0) return NULL;
    Item *latest_arr = (Item *)getData(itemArray, getOffset(itemArray), ITEM);
    DynamicArray *pos_items = fetchMultiPositions(itemArray, cmpTransitionedSymbol, (Data *)&expectedSymbol, ITEM);
    for (int i = 0; i < getNumElements(pos_items); i++) {
        Item *item = (Item *)getData(itemArray, i, ITEM);//sus *pos - 1
        if (latest_arr->transitionedSymbol != expectedSymbol) continue;
        if (isExsistingItem(item, prodArray)) return item;
    }
    
    destroyDynamicArray(pos_items);
    return NULL;
}

Item *setItem(DynamicArray *itemArray, int transitionedSymbol, DynamicArray *fetchedProdArray) {
    Item *new_item = (Item *)malloc(sizeof(Item));
    new_item->stateId = getNumElements(itemArray);
    new_item->transitionedSymbol = transitionedSymbol;
    new_item->Productions = (Production *)fetchedProdArray;
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
        DynamicArray *fetchedSymbolArray = createDynamicArray(10, INT);
        int size = ARRAY_LENGTH(productions) * sizeof(Production);
        DynamicArray *duplicatedProdsArray = createDynamicArray(size + 1, PRODUCTION);
        
        setUpDupliProd(duplicatedProdsArray, fetchedProdArray);
        
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

