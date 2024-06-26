#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include "token.h"
#include "AST.h"

#include "variable.h"
#include "array.h"

#include "debug.h"

#include "type.h"
#include "expr.h"

bool isReservedWord(char *name);

Node *declareVariableNode(Token **cur, Type *type, Var *vars, int *cur_offset);
Node *declareAssignVariableNode(Token **cur, Type *type, Var *vars, int *cur_offset);

Node *variableNode(Token **cur, Var *vars);

#endif