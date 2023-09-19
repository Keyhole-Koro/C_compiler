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

/*
 deprioritize()
 fetchedProdArray:
 duplicatedProdsArray: 0 1 2 3 4 5
 originalProd: 0 1 2 3 4 5
 ----
 fetchedProdArray: 2
 duplicatedProdsArray: 0 1 5 3 4
 originalProd: 0 1 2 3 4 5//doesnt be changed
 */

/*
 swapElement()
 fetchedSymbols: E T F
 F apperd
 ----
 fetchedSymbols: F T E
 */

DynamicArray *createDynamicArray(int initialCapacity, bool ifModifiable, Type type) {
    DynamicArray* arr = malloc(sizeof(DynamicArray));
    if (arr == NULL) error("Memory allocation failed\n");
    arr->data = (Data **)malloc(sizeof(Data *)*initialCapacity);
    if (arr->data == NULL) error("Memory allocation failed\n");
    arr->type = type;
    arr->offset = 0;
    arr->capacity = initialCapacity;
    arr->modifiable = ifModifiable;
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
    if (arr->modifiable == false) error("not allowed to be modified: swapElement\n");
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

void swapWithLastElement(DynamicArray *arr, int pos, Type type) {
    swapElement(arr, pos, getOffset(arr), type);
}


void destroyDynamicArray(DynamicArray* arr) {
    free(arr->data);
    free(arr);
}
//add safety
bool cmpStateId(Data *data, Data *expectedValue) {
    Item *item = (Item *)data;
    int *compedValue = (int*)expectedValue;
    return item->stateId == *compedValue;
}

bool cmpTransitionedSymbol(Data* data, Data* expectedValue) {
    Item *item = (Item *)data;
    int *compedValue = (int*)expectedValue;
    return item->transitionedSymbol == *compedValue;
}

int getKey(Data *data, Type type) {
    if (type != PRODUCTION) error("type mismatch: getKey\n");
    Production *prod = (Production *)data;
    return prod->key;
}


Data *getData(DynamicArray *arr, int pos, Type type) {
    if (type != arr->type) error("type mismatch: getData\n");

    if (pos < 0 || pos >= arr->offset) error("Index out of bounds: getData\n");

    return arr->data[pos];
}

void removeLastElement(DynamicArray *arr) {
    free(arr->data[arr->offset]);
    arr->offset--;
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
    DynamicArray *dArr = createDynamicArray(getNumElements(arr), false, INT);
    int offset = 0;
    for (int i = 0; i < arr->offset; i++) {
        Data *d = getData(arr, i, type);
        if (customCmp(d, expectedValue)) append(dArr, &i, INT);
    }
    return dArr;
}

DynamicArray *duplicateArray(DynamicArray *originalArr, bool ifModifiable) {
    Type type = originalArr->type;
    DynamicArray *duplicatedArray = createDynamicArray(getNumElements(originalArr), ifModifiable, type);

    for (int i = 0; i < getNumElements(originalArr); i++) {
        Data *copied_data = (Data *)malloc(getDataSize(type));
        *copied_data = *getData(originalArr, i, type);
        append(duplicatedArray, copied_data, type);
    }

    return duplicatedArray;
}


void deprioritizeArray(DynamicArray *arr, int pos_deprioritized, Type type) {
    if (type != arr->type) error("Type mismatch: deprioritizeArray\n");
    if (getNumElements(arr) >= 2) return;
    swapWithLastElement(arr, pos_deprioritized, type);
    
    removeLastElement(arr);
}

int calculateSetHash(DynamicArray *array, int (referentElement)(Data*, Type type), Type type) {
    if (type != PRODUCTION) error("type mismatch: calculateSetHash\n");
    int hash = 0;
        
    for (int i = 0; i < getNumElements(array); i++) {
        Data *data = getData(array, i, type);
        hash ^= referentElement(data, type);
    }

    return hash;
}

//this function sees the addresses but the value
//void eliminateOverlap(DynamicArray *arr) {
    
//}

Production *getProd_Item(Item *item, Type type) {
    if (type != ITEM) error("type mismatch: getProd_Item\n");
    Production *prod = item->Productions;
    return prod;
}
