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

extern Production *dummy_prod;
extern Item *dummy_item;

void appendCopiedOriginalProd(DynamicArray *arr, int key, Type type);

bool isNonTerminal(int symbol);
int getLeftFromProd(Data *prod, Type type);
int getCur_symbol(Data *data);
bool cmpSymbol(Data *data, Data *expectedValue);
bool cmpHash_keysOfProds(Data *data, Data *expectedValue);

void removeElement(DynamicArray *arr, int i, Type type);


void updateCur_Symbol(DynamicArray *prodArr, Type type);
DynamicArray *insertProds();
DynamicArray *setUpOriginalProd();

bool isEndProd(DynamicArray *prodArr);


#endif
