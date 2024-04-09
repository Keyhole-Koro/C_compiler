#include "LR1tableGenerator.h"

void printProd(DynamicArray *arr) {
    if (arr->type->name != "production") error("type mismatch: printProd\n");

    printf("--------\n|inside of a prod\n");
    printf("|-size: %d\n", getArraySize(arr));
    printf("|-overlap: ");
    arr->ifAllowOverlap ? printf("allowed\n"):printf("not alloweded\n");
    printf("|-modifing: ");
    arr->allowModifying ? printf("allowed\n") : printf("not allowed\n");
    for (int i = 0; i < getArraySize(arr); i++) {
        Production *prod = (Production *)retriveData(arr, i, arr->dataType);
        printf("|prod key left readPosition cur_symbol: %d %d %d %d\n", prod->key, getLeftFromProd((Data*)prod, arr->dataType), prod->readPosition, prod->cur_symbol);
    }
    printf("--------\n");
}