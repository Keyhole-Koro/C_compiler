#ifndef UTILITIES_H
#define UTILITIES_H

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))
#define SIZE_OF_A_MEMORY (8)

#include <stdio.h>
#include <stdlib.h>

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
    int transitionedSymbol;
    Production *Productions;
    int hashed_keys;//of keys of productions
    Item *transitionDestinations;
};


typedef enum {
    INT = 128,
    PRODUCTION,
    ITEM,
} Type;

void initializeItem(Item *item);

void initializeProduction(Production *prod);

size_t getDataSize(Type tp);

void error(char ch[]);


Item *setItem(int stateId, int transitionedSymbol);

Production dummy_prod_instance = {-1, -1, {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, -1, -1};
Production *dummy_prod = &dummy_prod_instance;

Item dummy_item_instance = {-1, -1, NULL, -1, NULL};
Item *dummy_item = &dummy_item_instance;

#endif
