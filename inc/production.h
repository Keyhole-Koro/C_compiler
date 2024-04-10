#ifndef PRODUCT_H
#define PRODUCT_H

#include "dataTypeHandler.h"

#include "expr.h"
#include "token.h"
#include "LR1_token.h"
#include "utils.h"

typedef struct {
    int key;
    int readPosition;
    Expr *expr;
    symbol_s cur_symbol;
} Production;

extern Production *dummy_prod;

Production *initializedProduction();

void updateCur_symbol(Production *prod);
int lookAheadSymbol(Production *prod);
int getKeyFromProd(void *data, DataType *dataType);
int getLeftFromProd(void *data, DataType *dataType);
int getRightFromProd(void *data, int index, DataType *dataType);
int getCur_symbol(void *data, DataType *dataType);
int getCur_symbolForOverlapHandler(void *data, DataType *dataType);

bool cmpCur_symbol(void *data, void *expectedData, DataType *dataType);
bool cmpLeftFromProd(void *data, void *expectedValue, DataType *dataType);

#endif