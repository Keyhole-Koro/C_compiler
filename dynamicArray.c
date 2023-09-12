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

DynamicArray* createDynamicArray(int initialCapacity, Type type) {
    DynamicArray* arr = malloc(sizeof(DynamicArray));
    if (arr == NULL) error("Memory allocation failed\n");
    arr->data = malloc(initialCapacity * getDataSize(type));
    if (arr->data == NULL) error("Memory allocation failed\n");
    arr->type = type;
    arr->offset = 0;
    arr->capacity = initialCapacity;
    return arr;
}

void append(DynamicArray* arr, void* element, Type type) {
    if (type != arr->type) error("type mismatch\n");

    if (arr->offset == arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * getDataSize(type));
        if (arr->data == NULL) error("Memory allocation failed\n");
    }
    arr->data[arr->offset++] = *((Data*)element);
}

//swqp
void swapElement(DynamicArray* arr, int pos1, int pos2, Type type) {
    if (type != arr->type) error("types don't match");


    if (pos1 < 0 || pos1 >= arr->offset || pos2 < 0 || pos2 >= arr->offset) error("Index out of bounds\n");

    Data* element1 = getData(arr, pos1, type);
    Data* temp_element1 = element1;
    Data* element2 = getData(arr, pos2, type);
    element1 = element2;
    element2 = temp_element1;
}

void destroyDynamicArray(DynamicArray* arr) {
    free(arr->data);
    free(arr);
}
//add safety
bool cmpStateId(Data* data, Data* expectedValue) {
    Item* item = (Item*)data;
    int* compedValue = (int*)expectedValue;
    return item->stateId == *compedValue;
}

bool cmpTransitionedSymbol(Data* data, Data* expectedValue) {
    Item *item = (Item*)data;
    int *compedValue = (int*)expectedValue;
    return item->transitionedSymbol == *compedValue;
}

Data* getData(DynamicArray* arr, int pos, Type type) {
    if (type != arr->type) error("type mismatch");

    if (pos < 0 || pos >= arr->offset) error("Index out of bounds\n");

    return &(arr->data[pos]);
}

int getOffset(DynamicArray* arr) {
    return arr->offset
}
//this only fetch one data but muiltiple
int fetchPosition(DynamicArray* arr, bool (customCmp)(Data*, Data*), Data* expectedValue, Type type) {
    if (type != arr->type) error("Type mismatch");
    Data* d;
    for (int i = 0; i < arr->offset; i++) {
        d = getData(arr, i, type);
        if (customCmp(d, expectedValue)) return i;
    }
    error("Not found\n");
    return -1;
}
