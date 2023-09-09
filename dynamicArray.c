    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #define "utilities.h"
    #include "commonData.h"
    #include "dynamicArray.h"

    DynamicArray createDynamicArray(int initialCapacity) {
        DynamicArray arr;
        arr.data = malloc(initialCapacity * elementSize);
        if (arr.data == NULL) error("Memory allocation failed\n");
        
        arr.offset = 0;
        arr.capacity = initialCapacity;
        return arr;
    }

    void append(DynamicArray *arr, const void *element, Type type) {
        if (type != arr->type) error("types don't match");
        
        if (arr->offset == arr->capacity) {
            arr->capacity *= 2;
            arr->data = realloc(arr->data, arr->capacity * getDataSize(type));
            if (arr->data == NULL) error("Memory allocation failed\n");
            
        }
        arr->data[offset++] = element;
    }

    void destroyDynamicArray(DynamicArray *arr) {
        free(arr->data);
        arr->data = NULL;
        arr->offset = 0;
        arr->capacity = 0;
    }

    void fetchData(DynamicArray *arr, void *data, const int pos, Type type) {
        if (type != arr->type) error("types don't match");
        
        if (pos < 0 || pos >= arr->offset) error("Index out of bounds\n");
        
        data = arr->data[pos];
    }

    void error(const char ch[]) {
        fprintf(stderr, "%s", ch);
        exit(1);
    }

    #if 0
    int convertBytesToInt(const char *bytes) {
        int intenger = 0;
        
        for (int i = 0; i < SI; i++) {
            intenger = (intenger << 8) | (bytes[i] & 0xFF);
        }
        
        return intenger;
    }

    Item convertBytesToItem(const char *bytes) {
        Item item;
        int offset = 0;

        item.stateId = *((int *)(bytes + offset));
        offset += sizeof(int);

        item.transitionedSymbol = *((int *)(bytes + offset));
        offset += sizeof(int);

        item.Productions = (Production **)malloc(sizeof(Production *) * SIZE_OF_A_MEMORY);
        for (int i = 0; i < SIZE_OF_A_MEMORY; i++) {
            item.Productions[i] = (Production *)(*((int *)(bytes + offset)));
            offset += SIZE_OF_A_MEMORY;
        }

        item.transitionDestinations = (Item **)malloc(sizeof(Item *) * SIZE_OF_A_MEMORY);
        for (int i = 0; i < SIZE_OF_A_MEMORY; i++) {
            item.transitionDestinations[i] = (Item *)(*((int *)(bytes + offset)));
            offset += SIZE_OF_A_MEMORY;
        }

        return item;
    }
    #endif
