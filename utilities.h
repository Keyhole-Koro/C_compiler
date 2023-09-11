#ifndef UTILITIES_H
#define UTILITIES_H

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))
#define SIZE_OF_A_MEMORY (8)

#include <stddef.h>

//#include "commonData.h"

typedef struct {
    int left;
    int *right; // modify later
    int readPosition;
} Production;

typedef struct Item {
    int stateId;
    int transitionedSymbol;
    Production** Productions;
    struct Item** transitionDestinations;
} Item;


typedef enum {
    INT = 128,
    PRODUCTION,
    ITEM,
} Type;

size_t getDataSize(Type tp);

#endif
