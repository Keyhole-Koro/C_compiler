#ifndef ASM_CONDI_EXPR_H
#define ASM_CONDI_EXPR_H

#include <stdio.h>

#include "AST.h"

#include "debug.h"

#include "variable.h"

void conditional_exprGen(Node *condi, Var *vars);

bool isLogicalOperator(Node *node);
bool isCmparisionOperatorNode(Node *node);

#endif