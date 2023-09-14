// dynamic_array.h
#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

//#include "commonData.h"
#include "utilities.h"

typedef union {
    int **intData;
    Production **producitonData;
    Item **itemData;
} Data;

typedef struct {
    Type type;
    Data **data;
    int offset;
    int capacity;
} DynamicArray;

DynamicArray* createDynamicArray(int initialCapacity, Type type);
void append(DynamicArray* arr, void* element, Type type);
int fetchPosition(DynamicArray* arr, bool (customCmp)(Data*, Data*), Data* expectedValue, Type type);
Data *getData(DynamicArray* arr, int pos, Type type);
void destroyDynamicArray(DynamicArray* arr);
int getOffset(DynamicArray* arr);
int getNumElements(DynamicArray *arr);
void swapElement(DynamicArray* arr, int pos1, int pos2, Type type);
#endif

