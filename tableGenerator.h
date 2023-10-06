#ifndef TABLEGENERATOR_H
#define TABLEGENERATOR_H

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamicArray.h"
#include "utilities.h"

#include <unistd.h>


Production empty_prod_instance = {-1, -1, {-1}, -1, -1};
Production *empty_prod = &empty_prod_instance;

Item null_item_instance;
Item *null_item = &null_item_instance;
null_item->stateId = -1;


void appendCopiedOriginalProd(DynamicArray *arr, int key, Type type) 

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

void appendProdLeftIs(DynamicArray *fetchedProdArray, DynamicArray *originalProdArray, unsigned char *ifExistingArray, int expectedSymbol);
DynamicArray *gatherNessesaryProds(DynamicArray *fetchedProdArray, DynamicArray *fetchedSymbolArray);
DynamicArray *setUpDupliSortUpdateProd(DynamicArray *oldProdArray, Type type);

bool isEndProd(DynamicArray *prodArr);
DynamicArray *extractProd(DynamicArray *copiedProdArray, int expectedSymbol);
Item *transistedProds_Item(DynamicArray *itemArray, DynamicArray *ProdArray, DynamicArray *symbolArray);
bool isExsistingItem(Item *referentItem, Production *prod);
Item *fetchItem(DynamicArray *itemArray, DynamicArray *prodArray, int expectedSymbol);
Item *createItem(DynamicArray *itemArray, DynamicArray *fetchedProdArray, int expectedSymbol);

Item *createItem(DynamicArray *itemArray, DynamicArray *fetchedProdArray, int expectedSymbol);
DynamicArray *setUpDupliSortUpdateProd(DynamicArray *oldProdArray, Type type);

#endif