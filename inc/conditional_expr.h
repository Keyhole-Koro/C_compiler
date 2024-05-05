#ifndef CONDI_EXPR_H
#define CONDI_EXPR_H

#include "token.h"
#include "AST.h"

#include "debug.h"

#include "type.h"
#include "expr.h"

Node *condtionalExprNode(Token **cur, Var *vars);

#endif