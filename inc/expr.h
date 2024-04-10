#ifndef EXPR_H
#define EXPR_H

#include "dataTypeHandler.h"

#include "token.h"
#include "utils.h"

typedef struct {
    int key;
    int left;
    int *right;
} Expr;

Expr *initializedExpr();

void registerExpr(Token left, Token *right);

#endif