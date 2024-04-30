#ifndef EXPR_H
#define EXPR_H

#include "token.h"
#include "AST.h"

#include "identifier.h"
#include "variable.h"

#include "debug.h"

Node *exprNode(Token **cur, Var *targetVar);

bool isOperator(AST_Type type);

#endif