#ifndef EXPR_H
#define EXPR_H

#include "token.h"
#include "AST.h"

#include "debug.h"

Node *expr(Token **cur);

bool isOperator(AST_Type type);

#endif