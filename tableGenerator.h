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

int getCur_symbol(Data *data, Type type);

void updateCur_Symbol(DynamicArray *prodArr, Type type);
DynamicArray *insertProds();
DynamicArray *setUpOriginalProd();

void constructItem(DynamicArray *itemArray, DynamicArray *fetchedProdArray, int *hashedStateIdOfItem_transit, int expected_symbol);

bool isClosure(DynamicArray *prodArr);


#endif
