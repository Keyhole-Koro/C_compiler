#ifndef UTILITIES_H
#define UTILITIES_H

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))
#define SIZE_OF_A_MEMORY (8)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Production Production;

struct Production {
    int key;
    int left;
    int right[10]; // modify later
    int readPosition;
    int cur_symbol;
};

typedef struct Item Item;

struct Item {
    int stateId;
    int readSymbol;
    Production *Productions;
    int hashed_keys;//of keys of productions
    Item *transitItems;
};

extern int *dummy_int;
extern Production *dummy_prod;
extern Item *dummy_item;

typedef enum {
    INT = 128,
    PRODUCTION,
    ITEM,
} Type;

Item *initializeItem();
Production *initializeProduction();

size_t getDataSize(Type tp);

//Data *getDummy(Type type);

void error(char ch[]);

Item *createItem(int stateId, int readSymbol, Production *prod, int hashed_keys, Item *item);

#endif
