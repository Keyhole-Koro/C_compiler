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
#include "utilities.h"

void appendCopiedOriginalProd(DynamicArray *arr, int key, Type type);

bool isNonTerminal(int symbol);
int cur_symbolGetter(Data *prod, Type type);
int leftGetter(Data *prod, Type type);
int getCurSymbol(Data *data);
bool cmpSymbol(Data *data, Data *expectedValue);

bool isSymbolOverlap(DynamicArray *symbolArr, int symbol, Type type);
void removeElement(DynamicArray *arr, int i, Type type);
void prioritizeSymbol(DynamicArray *fetchedSymbolArray, int symbol);

void appendSymbol(DynamicArray *SymbolArray, int cur_symbol);

void updateCur_Symbol(DynamicArray *prodArr, Type type);
DynamicArray *insertProds();
DynamicArray *setUpOriginalProd();

bool isEndProd(DynamicArray *prodArr);

DynamicArray *setUpDupliSortUpdateProd(DynamicArray *oldProdArray, Type type);

#endif
