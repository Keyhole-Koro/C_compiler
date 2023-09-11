#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "commonData.h"
#include "dynamicArray.h"
#include "utilities.h"

void error(const char ch[]) {
    fprintf(stderr, "%s", ch);
    exit(1);
}

DynamicArray *createDynamicArray(int initialCapacity, Type type) {
    DynamicArray *arr = malloc(sizeof(DynamicArray));
    if (arr == NULL) error("Memory allocation failed\n");
    arr->data = malloc(initialCapacity * getDataSize(type));
    if (arr->data == NULL) error("Memory allocation failed\n");
    arr->type = type;
    arr->offset = 0;
    arr->capacity = initialCapacity;
    return arr;
}

void append(DynamicArray *arr, void *element, Type type) {
    if (type != arr->type) error("type mismatch\n");

    if (arr->offset == arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * getDataSize(type));
        if (arr->data == NULL) error("Memory allocation failed\n");
    }
    arr->data[arr->offset++] = *((Data*)element);
}

void destroyDynamicArray(DynamicArray *arr) {
    free(arr->data);
    free(arr);
}
//add safety
bool cmpStateId(Data *data, Data *expectedValue) {
    Item *item = (Item *)data;
    int *compedValue = (int *)expectedValue;
    return item->stateId == *compedValue;
}

bool cmpTransitionedSymbol(Data *data, Data *expectedValue) {
    Item *item = (Item *)data;
    int *compedValue = (int *)expectedValue;
    return item->transitionedSymbol == *compedValue;
}

Data *getData(DynamicArray *arr, int pos, Type type) {
    if (type != arr->type) error("type mismatch");

    if (pos < 0 || pos >= arr->offset) error("Index out of bounds\n");
        
    return &(arr->data[pos]);
}

int getOffset(DynamicArray *arr) {
    return arr->offset
}
//this only fetch one data but muiltiple
int fetchPosition(DynamicArray *arr, bool (customCmp)(Data *, Data *), Data *expectedValue, Type type) {
    if (type != arr->type) error("Type mismatch");
    Data *d;
    for (int i = 0; i < arr->offset; i++) {
        d = getData(arr, i, type);
        if (customCmp(d, expectedValue)) return i;
    }
    error("Not found\n");
    return -1;
}
