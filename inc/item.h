#ifndef ITEM_H
#define ITEM_H

#include "dataTypeHandler.h"

#include "production.h"
#include "utils.h"

typedef struct {
    int stateId;
    int readSymbol;
    int arraySize;
    int hashed_keys; // of keys of productions
    Production **Productions;
} Item;

extern Item *dummy_item;

Item *initializedItem();
Item *createItem(int stateId, int readSymbol, int arraySize, Production **prod, int hashed_keys);

void updateHashedStateId(int *updatedId, int stateId);

bool cmpStateId(void *data, void *expectedData, DataType *dataType);
bool cmpReadSymbol(void *data, void *expectedValue, DataType *dataType);
bool cmpHash_keyOfProdsInItem(void *data, void *expectedData, DataType *dataType);
bool cmp_readSymbolHash_keyOfProdsInItem(void *data, void *expectedData, DataType *dataType);

int getHash_keyOfProdsInItem(void *data, DataType *dataType);
#endif