#ifndef UTILITIES_H
#define UTILITIES_H

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))
#define SIZE_OF_A_MEMORY (8)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Expr Expr;

struct Expr {
    int key;
    int left;
    int right[10];
};

typedef struct Production Production;

struct Production {
    int key;
    Expr *expr;
    int readPosition;
    int cur_symbol;
};

typedef struct Item Item;

struct Item {
    int stateId;
    int readSymbol;
    int arraySize;
    Production **Productions;
    int hashed_keys;//of keys of productions
};

extern int *dummy_int;
extern Production *dummy_prod;
extern Item *dummy_item;

typedef enum {
    INT = 128,
    PRODUCTION,
    ITEM,
    EXPR,
} Type;

Item *initializeExpr();
Item *initializeItem();
Production *initializeProduction();

size_t getDataSize(Type tp);

//Data *getDummy(Type type);

void error(char ch[]);

void updateHashedStateId(int *updatedId, int stateId);

Item *createItem(int stateId, int readSymbol, int arraySize, Production **prod, int hashed_keys);

#endif
