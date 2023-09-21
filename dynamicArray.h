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
    bool modifiable;
} DynamicArray;

DynamicArray *createDynamicArray(int initialCapacity, bool ifModifiable, Type type);
void append(DynamicArray* arr, void* element, Type type);
int fetchPosition(DynamicArray* arr, bool (customCmp)(Data*, Data*), Data* expectedValue, Type type);
Data *getData(DynamicArray* arr, int pos, Type type);
void destroyDynamicArray(DynamicArray* arr);

int getOffset(DynamicArray* arr);
int getNumElements(DynamicArray *arr);
void removeLastElement(DynamicArray *arr);

DynamicArray *duplicateArray(DynamicArray *originalArr, bool ifModifiable);
void swapRemoveElement(DynamicArray *arr, int pos_deprioritized, Type type);

void swapElement(DynamicArray* arr, int pos1, int pos2, Type type);
void swapWithLastElement(DynamicArray *arr, int pos, Type type);

void eliminateOverlap(DynamicArray *arr, int (referentElement)(Data*, Type), Type type);

DynamicArray *fetchMultiPositions(DynamicArray *arr, bool (customCmp)(Data*, Data*), Data* expectedValue, Type type);

bool cmpTransitionedSymbol(Data* data, Data* expectedValue);
int getKey(Data *, Type type);

int calculateSetHash(DynamicArray *array, int (referentElement)(Data*, Type), Type);

Production *getProd_Item(Item *item, Type);



unsigned char *createNormalUnsignedCharArray(int size);
void initializeUnsignedCharArraywithZero(unsigned char *intArray);

#endif
