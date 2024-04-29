#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include "token.h"
#include "AST.h"

#include "debug.h"

#include "type.h"
#include "expr.h"

typedef struct Var Var;

struct Var {
    char *name;
    Type *type;
    int offset;
    Var *next;
};

Node *declareVariableNode(Token **cur, Type *type, Var *vars, int *cur_offset);
Node *assignNode(Token **cur, Node *left);
Node *variableNode(Token **cur, Var *vars);

#endif