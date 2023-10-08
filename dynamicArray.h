// dynamic_array.h
#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utilities.h"

typedef union {
    int **intData;
    Production **producitonData;
    Item **itemData;
} Data;

typedef struct {
    Type type;
    Data **data;
	bool ifOverlap;//true allows overlapping //!be careful not to handle big numbers
	unsigned char *ifOverlapArray;//its array contains 0 1 once value appears its contained the corresponding index by 1
    int (*referentMember)(Data*, Type);
    int offset;
    int capacity;
    bool modifiable;
} DynamicArray;

extern Data *dummy_data;

DynamicArray *createDynamicArray(int initialCapacity, bool ifModifiable, int (*referentMember)(Data*, Type), Type type);
void normalReallocateDynamicArray(DynamicArray *arr);
void append(DynamicArray *arr, void *element, Type);
void appendCopy(DynamicArray *arr, void *element, Type);

int fetchPosition(DynamicArray* arr, bool (customCmp)(Data*, Data*), Data* expectedValue, Type type);
Data *retriveData(DynamicArray* arr, int pos, Type type);
void destroyDynamicArray(DynamicArray* arr);

int dummy_member(Data *data, Type type);
int getArrayOffset(DynamicArray* arr);
int getArraySize(DynamicArray *arr);
void removeLastElement(DynamicArray *arr);

DynamicArray *fetchCommonElements(DynamicArray *arr, bool (customCmp)(Data*, Data*), Data *expected_data, Type type);

DynamicArray *cloneArray(DynamicArray *originalArr, bool ifModifiable, int (*referentMember)(Data*, Type));
void swapRemoveElement(DynamicArray *arr, int pos_deprioritized, Type type);

void swapElement(DynamicArray* arr, int pos1, int pos2, Type type);
void swapWithLastElement(DynamicArray *arr, int pos, Type type);

void eliminateOverlap(DynamicArray *arr, int (referentMember)(Data*, Type), int start_point, Type type);

int getProdKey(Data *data, Type type);
Item *fetchMatchingData(DynamicArray *itemArray, DynamicArray *expectedProdArray, int expectedSymbol);

bool cmpTransitionedSymbol(Data* data, Data* expectedValue);

int calculateArrayHash(DynamicArray *array, int (referentElement)(Data*, Type), Type);

Production *getProdFromItem(Item *item, Type);

bool hasOverlap_Insert(DynamicArray *arr, int index);

bool ifExistInOverlap(DynamicArray *arr, int index);
unsigned char *createEmptyUnsignedCharArray(int size);
void initializeUnsignedCharArraytoZero(unsigned char *intArray);
unsigned char *initializeOverlapArray(DynamicArray *existingElementArr, int (referentMember)(Data*, Type), Type type);
void appendAtIndexOverlapArray(DynamicArray *arr, int index);
void destoryUnsignedCharArray(unsigned char *arr);

bool cmpInt(Data* data1, Data* data2);
int getIntFromData(Data * data, Type type);

int qsortPartition(DynamicArray *arr, int (referentFunc)(Data *, Type), int low, int high, Type type);
void quickSort(DynamicArray *arr, int (referentFunc)(Data *, Type), int low, int high, Type type);

#endif
