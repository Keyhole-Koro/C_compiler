#include "utilities.h"

#ifndef COMMON_DATA_H
#define COMMON_DATA_H
typedef struct {
    int left;
    int right[10];//modify later
    int readPosition;
} Production;

typedef struct {
    int stateId;
    int transitionedSymbol;
    Production **Productions;
    	Item **transitionDestinations;
} Item;

#endif
