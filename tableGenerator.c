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
    {EXPR, {EXPR, END}, 0},
    {EXPR, {EXPR, ADD, TERM}, 0},
    {EXPR, {EXPR, SUB, TERM}, 0},
    {EXPR, {TERM}, 0},
    {TERM, {TERM, MUL, FACTOR}, 0},
    {TERM, {TERM, DIV, FACTOR}, 0},
    {TERM, {FACTOR}, 0},
    {FACTOR, {L_PARENTHESES, EXPR, R_PARENTHESES}, 0},
    {FACTOR, {NUM}, 0}
};

//implement calloc later
//use NULL indicates the end

//implement array functions makeArray append getArray

Production *getProdStartWith(int symbol) {//when just after dot is non-terminal, this is called
    Production *prodArray = (Production *)malloc(ARRAY_LENGTH(productions) * sizeof(Production));
    int numMatches = 0;

    for (int i = 0; i < ARRAY_LENGTH(productions); i++) {
        if (productions[i].left == symbol) {
            prodArray[numMatches++] = &productions[i];
        }
    }
    prodArray[numMatches] = (Production){0, NULL, 0};
    prodArray = (Production *)realloc(prodArray, (numMatches) * sizeof(Production));
    return prodArray;//you cant return stack values
}
/*
Production *fetchProductions(Production prod) {//give specific name
    
}
*/

bool isOverlap(int *array, int size, int symbol) {
    for (int i = 0; i < size; i++) {
        if (*array[i] == symbol) return false;
    }
    return true;
}

int *getSymbol(Production *prods) {
    int *symbolArray[] = {};
    for (int i = 0; i < ARRAY_LENGTH(prods); i++) {
        if (isOverlap(prods, ARRAY_LENGTH(prods), prods[i])) {
            symbolArray[ARRAY_LENGTH(symbolArray)] = prods[i];
        }
    }
    return symbolArray;
}

Production *gatherProdRightStartWith(int *non_terminals[]) {    
    Production *prodArray = (Production *)malloc(fetchedProds * sizeof(Production));
    if (prodArray == NULL) return NULL;
    return prodArray;
}

void removeElement(Production arr[], int i) {
    arr[i] = (Production){NULL, {NULL}, NULL};
}

//this one extract single kind of prods
int appendProdLeftIs(DynamicArray *prodArray, DynamicArray *copiedProdsArray, int expectedSymbol, int beginning_point) {
    for (int i = beginning_point; i > 0; i--) {
        if (copiedProdsArray[i]->left == expectedSymbol) {
            append(prodArray, productions[i], PRODUCTION);
            remove(copiedProdsArray, i);
            int no_meaning = appendProdLeftIs(prodArray, copiedProdsArray, expectedSymbol, i - 1);
        }
    }
    return 0;
}

//this one extracts multiple kind of prods
void *extractNessesaryProds(DynamicArray *prodArray, Production *copiedProdsArray) {
    for (int i = 0; i < getOffset(prodArray) + 1; i++) {
        if (prodArray[i].left == NULL) continue;
                        
        int pos = prodArray[i]->readPosition;
        int symbol = prodArray[i]->right[pos];
        
        if (!isNonTerminal(symbol)) continue;
                
        int no_meaning = appendProdLeftIs(prodArray, copiedProdsArray, symbol, getOffset(copiedProdsArray));
    }
}
//the copying prod takes much time
Item *createItem(DynamicArray *darr, Production *requiredProdArray, int readSymbol){
    Production *copiedProdsArray = (Production *)malloc(ARRAY_LENGTH(Productions) * sizeof(Production));
    *copiedProdsArray = productions;
    extractNessesaryProds(requiredProdArray, copiedProdsArray);
    
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
        
    append(darr, new_item, ITEM);
    
    free(copiedProd);
}

int main() {
    DynamicArray *itemArray = createDynamicArray(10, ITEM);
    
    DynamicArray *requiredProdArray = createDybamicArray(10, PRODUCTION);
    append(requiredProdArray, &productions[0], PRODUCTION);

    Item *first_item = createItem(itemArray, requiredProdArray, EXPR);//temporary
    
    
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
