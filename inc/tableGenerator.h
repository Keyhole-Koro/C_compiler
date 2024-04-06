#ifndef TABLEGENERATOR_H
#define TABLEGENERATOR_H

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "dynamicArray.h"
#include "utils.h"

extern Expr *dummy_expr;
extern Production *dummy_prod;
extern Item *dummy_item;

typedef int state_id;

int getLeftFromProd(Data *data, Type type);


Expr *getExprWithKey(int key);
DynamicArray *presetProductions();

void readOneSymbol(DynamicArray *prodArr);

DynamicArray *Clone_readOne_Cur_symbol(DynamicArray *oldProdArray, Type type);

bool isClosureItem(DynamicArray *prodArr);

DynamicArray *extract_appnedCur_symbol(DynamicArray *prodArray, DynamicArray *appendedArray, bool ifAllowNonTerminal);

DynamicArray *extract_listCur_symbol(DynamicArray *prodArray, bool ifAllowNonTerminal);

Item *findExistingItem(DynamicArray *itemArray, DynamicArray *prodArray, int expected_readSymbol);
state_id separateProds(DynamicArray *itemArray, DynamicArray *prodArray);
DynamicArray *gatherProdswithSymbols(DynamicArray *prodArray);

state_id constructItem(DynamicArray *itemArray, DynamicArray *prodArray, int expected_symbol);

#endif
