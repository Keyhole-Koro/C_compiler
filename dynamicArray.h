// dynamic_array.h
#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "commonData.h"
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
	int start_point;
    int offset;
    int capacity;
    bool modifiable;
} DynamicArray;

DynamicArray *createDynamicArray(int initialCapacity, bool ifModifiable, bool ifOverlap, Type type);
void normalReallocateDynamicArray(DynamicArray *arr);
void normalAppend(DynamicArray *arr, void *element, Type type);
void append(DynamicArray *arr, void *element, int (referentMember)(Data*, Type), Type type);
void appendCopied(DynamicArray *arr, void *element, int (referentMember)(Data*, Type), Type type);

int fetchPosition(DynamicArray* arr, bool (customCmp)(Data*, Data*), Data* expectedValue, Type type);
Data *getData(DynamicArray* arr, int pos, Type type);
void destroyDynamicArray(DynamicArray* arr);

int dummy_member(Data *data, Type type);
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

bool isOverlap_Insert_1(DynamicArray *arr, int index);

unsigned char *createNormalUnsignedCharArray(int size);
void initializeUnsignedCharArraywithZero(unsigned char *intArray);
unsigned char *setUpifOverlapArray(DynamicArray *existingElementArr, int (referentMember)(Data*, Type), Type type);

int qsortPartition(DynamicArray *arr, int (referentFunc)(Data *, Type), int low, int high, Type type);
void quickSort(DynamicArray *arr, int (referentFunc)(Data *, Type), int low, int high, Type type);

#endif
