// dynamic_array.h

#include "commonData.h"

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

typedef union {
    int *intData;
    Production *producitonData;
    Item *itemData;
} Data;

typedef struct {
    Type type;
    Data data;
    int offset;
    int capacity;
} DynamicArray;

DynamicArray createDynamicArray(int initialCapacity, size_t elementSize);
void append(DynamicArray *arr, const void *element);
void destroyDynamicArray(DynamicArray *arr);

#endif

