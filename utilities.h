#ifndef UTILITIES_H
#define UTILITIES_H

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))
#define SIZE_OF_A_MEMORY (8)

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "commonData.h"

typedef struct {
    int key;
    int left;
    int right[10]; // modify later
    int readPosition;
    int cur_symbol;
} Production;

typedef struct Item Item;

typedef struct Item {
    int stateId;
    int transitionedSymbol;
    Production *Productions;
    int hashed_keys;//of keys of productions
    Item *transitionDestinations;
} Item;


typedef enum {
    INT = 128,
    PRODUCTION,
    ITEM,
} Type;

void initializeItem(Item *item);

void initializeProduction(Production *prod);

size_t getDataSize(Type tp);

void error(char ch[]);


Item *setItem(DynamicArray *itemArray, int stateId, int transitionedSymbol, DynamicArray *fetchedProdArray);

Production *dummy_prod;
initializeProduction(dummy_prod);

Item *dummy_item;
initializeItem(dummy_item);

#endif
