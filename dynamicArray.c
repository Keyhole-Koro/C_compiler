#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "commonData.h"
#include "dynamicArray.h"
#include "utilities.h"

/*
DynamicArray*
|
|->Data **
     |     --------------
     | -> |ptr0|ptr1|ptr2|
           --------------
     ----    ----    ----
    |val0|  |val1|  |val2|
     ----    ----    ----
 value are separated
 */
DynamicArray *createDynamicArray(int initialCapacity, Type type) {
    DynamicArray* arr = malloc(sizeof(DynamicArray));
    if (arr == NULL) error("Memory allocation failed\n");
    arr->data = (Data **)malloc(sizeof(Data *)*initialCapacity);
    if (arr->data == NULL) error("Memory allocation failed\n");
    arr->type = type;
    arr->offset = 0;
    arr->capacity = initialCapacity;
    return arr;
}

void append(DynamicArray* arr, void *element, Type type) {
    if (type != arr->type) error("type mismatch: append\n");

    if (arr->offset == arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * getDataSize(type));
        if (arr->data == NULL) error("Memory allocation failed\n");
    }
    Data **copy_data = malloc(sizeof(*element));
    *copy_data = (Data *)element;
    arr->data[arr->offset++] = *copy_data;
    //memcpy(&(arr->data[arr->offset++]), element, getDataSize(type));
}

void swapElement(DynamicArray *arr, int pos1, int pos2, Type type) {
    if (type != arr->type) {
        error("type mismatch: swapElement\n");
        return;
    }

    if (pos1 < 0 || pos1 >= arr->offset || pos2 < 0 || pos2 >= arr->offset) {
        error("Index out of bounds: swapElement\n");
        return;
    }

    Data **data = arr->data;

    Data *temp_element = data[pos1];
    data[pos1] = data[pos2];
    data[pos2] = temp_element;
}


void destroyDynamicArray(DynamicArray* arr) {
    free(arr->data);
    free(arr);
}
//add safety
bool cmpStateId(Data *data, Data *expectedValue) {
    Item *item = (Item*)data;
    int *compedValue = (int*)expectedValue;
    return item->stateId == *compedValue;
}

bool cmpTransitionedSymbol(Data* data, Data* expectedValue) {
    Item *item = (Item*)data;
    int *compedValue = (int*)expectedValue;
    return item->transitionedSymbol == *compedValue;
}


Data *getData(DynamicArray *arr, int pos, Type type) {
    if (type != arr->type) error("type mismatch: getData\n");

    if (pos < 0 || pos >= arr->offset) error("Index out of bounds: getData\n");

    return arr->data[pos];
}

int getOffset(DynamicArray* arr) {
    return arr->offset -1 ;
}

int getNumElements(DynamicArray *arr) {
    return arr->offset;
}
//this only fetch one data but muiltiple
int fetchPosition(DynamicArray *arr, bool (customCmp)(Data*, Data*), Data* expectedValue, Type type) {
    if (type != arr->type) error("Type mismatch: fetchPosition\n");
    Data *d;
    for (int i = 0; i < arr->offset; i++) {
        d = getData(arr, i, type);
        if (customCmp(d, expectedValue)) return i;
    }
    error("Not found\n");
    return -1;
}

DynamicArray *fetchMultiPositions(DynamicArray *arr, bool (customCmp)(Data*, Data*), Data *expectedValue, Type type) {
    if (type != arr->type) error("Type mismatch: fetchPosition\n");
    DynamicArray *dArr = createDynamicArray(getNumElements(arr), INT);
    int offset = 0;
    for (int i = 0; i < arr->offset; i++) {
        Data *d = getData(arr, i, type);
        if (customCmp(d, expectedValue)) append(dArr, &i, INT);
    }
    return dArr;
}

//duplicate()
