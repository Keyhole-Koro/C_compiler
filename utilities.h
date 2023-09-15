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

typedef struct Item {
    int stateId;
    int transitionedSymbol;
    Production **Productions;
    struct Item **transitionDestinations;
} Item;


typedef enum {
    INT = 128,
    PRODUCTION,
    ITEM,
} Type;

size_t getDataSize(Type tp);

void error(char ch[]);

#endif
