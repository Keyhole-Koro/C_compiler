// dynamic_array.h
#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

//#include "commonData.h"
#include "utilities.h"

typedef union {
    int *intData;
    Production *producitonData;
    Item *itemData;
} Data;

typedef struct {
    Type type;
    Data *data;
    int offset;
    int capacity;
} DynamicArray;

DynamicArray *createDynamicArray(int initialCapacity, Type type);
void append(DynamicArray *arr, void *element, Type type);
void destroyDynamicArray(DynamicArray *arr);
int getOffset(DynamicArray *arr);

#endif

