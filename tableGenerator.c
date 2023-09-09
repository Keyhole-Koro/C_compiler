#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define "utilities.h"
#include "commonData.h"
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

/*
Production prod1 = { 1, NULL, 0 };
Production prod2 = { 2, NULL, 0 };
Item item1;
Item item2;
Production *productionArray[] = { &prod1, &prod2, NULL };
State *stateArray[] = { &item1, &item2, NULL };
State item3 = {, 3, productionArray, stateArray };
*/
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

//createFirst()
//getFirst()
//createFollow()
//getFollow()
//createState()
//sortRows(
//sortColumns()
//readItem()

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

int *extractNonTerminal(Production *prodArray[]) {
    int *nonTerminalArray = (int *)malloc(ARRAY_LENGTH(prodArray) * sizeof(int));
    if (nonTerminalArray == NULL) return NULL;
    int numNonTerminals = 0;

    for (int i = 0; i < ARRAY_LENGTH(prodArray); i++) {
        int pos = prodArray[i]->readPosition;
        int symbol = prodArray[i]->right[pos];

        if (symbol == EXPR || symbol == TERM || symbol == FACTOR && !isOverlap(nonTerminalArray, ARRAY_LENGTH(nonTerminalArray), symbol)) {//categorize non-terminal later
            nonTerminalArray[numNonTerminals++] = symbol;
        }
    }
    nonTerminalArray[numNonTerminals] = NULL;
    nonTerminalArray = (int *)realloc(nonTerminalArray, numNonTerminals * sizeof(int));
    return nonTerminalArray;
}

Production *getProdLeftIs(int non_terminals[]) {
    Production *matchedProdArray = (Production *)malloc(ARRAY_LENGTH(productions) * sizeof(Production));
    if (matchedProdArray == NULL) return NULL;
    int prods_index = -1;
    for (int i = 0; i < sizeof(non_terminals); i++) {
        for (int j = 0; j < ARRAY_LENGTH(productions) != NULL; j++) {
        if (productions[i].left == non_terminals[i]) {
            matchedProdArray[++prods_index] = productions[j];
        }
    }
    }
    matchedProdArray[++prods_index] = (Production){0, NULL, 0};
    matchedProdArray = (Production *)realloc(matchedProdArray, (prods_index + 1) * sizeof(Production));
    return matchedProdArray;
}

Production *gatherNeededProds(Production *referenced_prods[]) {
    int *non_terminals = extractNonTerminal(referenced_prods);
    if (*non_terminals == NULL) return NULL;
    int *fetched_prods;

    fetched_prods = getProdLeftIs(non_terminals);
    
    if (ARRAY_LENGTH(referenced_prods) == ARRAY_LENGTH(fetched_prods)) return fetched_prods;
    return gatherNeededProds(fetched_prods);
}

Item *createItem(Production *referenced_prods[], int readSymbol){
    Item *new_item = (Item *)malloc(sizeof(Item));
    
    Item *itemArray[] = {};
    int num_item = 0;

    Production *nessesaryProds = gatherNeededProds(referenced_prods);
    //have yet to implement dot
    int *symbols = getSymbol(prodArray);
    for (int i = 0; i < ARRAY_LENGTH(symbols); i++) {
        createItem();
    }
    
    new_item->stateId = numItems;
    new_item->transitionedSymbol = readSymbol;
    new_item->Productions = prodArray;
    new_item->transitionDestinations = itemArray;
    
    listItems[numItems++] = new_item;
    
    return new_item;
}

int main() {
    Production *referenced_prods[] = {};
    int num_referenced_prods = 0;
    referenced_prods[num_referenced_prods++] = &productions[0];
    referenced_prods[num_referenced_prods] = NULL;
    Item *first_item = createItem(referenced_prods, EXPR);//temporary
    
    
}

