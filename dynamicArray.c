#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commonData.h"
#include "dynamicArray.h"
#include "utilities.h"

void error(const char ch[]) {
    fprintf(stderr, "%s", ch);
    exit(1);
}

DynamicArray createDynamicArray(int initialCapacity, Type type) {
    DynamicArray arr;
    arr.data = malloc(initialCapacity * getDataSize(type));
    if (arr.data == NULL) error("Memory allocation failed\n");
    arr.type = type;
    arr.offset = 0;
    arr.capacity = initialCapacity;
    return arr;
}

void append(DynamicArray *arr, void *element, Type type) {
    if (type != arr->type) error("types don't match");

    if (arr->offset == arr->capacity) {
        arr->capacity *= 2;
        arr->data = realloc(arr->data, arr->capacity * getDataSize(type));
        if (arr->data == NULL) error("Memory allocation failed\n");
    }

    arr->data[arr->offset++] = *((Data*)element);
}


void destroyDynamicArray(DynamicArray *arr) {
    free(arr->data);
    arr->data = NULL;
    arr->offset = 0;
    arr->capacity = 0;
}

void fetchData(DynamicArray *arr, void **data, int pos, Type type) {
    if (type != arr->type) error("types don't match");

    if (pos < 0 || pos >= arr->offset) error("Index out of bounds\n");

    *data = &(arr->data[pos]);
}


int main() {
    // Create a dynamic array for integers
    DynamicArray intArray = createDynamicArray(10, sizeof(int));
    
    int intValue = 42;
    append(&intArray, &intValue, INT);
    
    int fetchedInt;
    fetchData(&intArray, (void**)&fetchedInt, 0, INT);
    
    printf("Fetched Integer: %d\n", fetchedInt);
    
    // Create a dynamic array for Productions
    DynamicArray productionArray = createDynamicArray(5, sizeof(Production));
    
    Production productionValue;
    productionValue.left = 10;
    append(&productionArray, &productionValue, PRODUCTION);
    
    Production fetchedProduction;
    fetchData(&productionArray, (void**)&fetchedProduction, 0, PRODUCTION);
    
    printf("Fetched Production: left=%d\n", fetchedProduction.left);
    
    // Create a dynamic array for Items
    DynamicArray itemArray = createDynamicArray(3, sizeof(Item));
    
    Item itemValue;
    itemValue.stateId = 20;
    append(&itemArray, &itemValue, ITEM);
    
    Item fetchedItem;
    fetchData(&itemArray, (void**)&fetchedItem, 0, ITEM);
    
    printf("Fetched Item: stateId=%d\n", fetchedItem.stateId);
    
    // Destroy the dynamic arrays
    destroyDynamicArray(&intArray);
    destroyDynamicArray(&productionArray);
    destroyDynamicArray(&itemArray);
    
    return 0;
}
