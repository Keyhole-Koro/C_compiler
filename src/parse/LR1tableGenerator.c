#include "LR1tableGenerator.h"

void printProd(DynamicArray *arr) {
    if (arr->dataType->name != "prod") error("type mismatch: printProd\n");

    printf("--------\n|inside of a prod\n");
    printf("|-size: %d\n", getArrayOffset(arr));
    printf("|-overlap: ");
    arr->allowOverlapping ? printf("allowed\n"):printf("not alloweded\n");
    printf("|-modifing: ");
    arr->allowModification ? printf("allowed\n") : printf("not allowed\n");
    for (int i = 0; i < getArrayOffset(arr); i++) {
        Production *prod = (Production *)retrieveData(arr, i, arr->dataType);
        printf("|prod key left readPosition cur_symbol: %d %d %d %d\n", prod->key, getLeftFromProd(prod, arr->dataType), prod->readPosition, prod->cur_symbol);
    }
    printf("--------\n");
}