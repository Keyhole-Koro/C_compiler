#ifndef VARIABLE_H
#define VARIABLE_H

#include "type.h"

#include "AST.h"

#include "pointer.h"

typedef struct Var Var;

struct Var {
    char *name;
    Type *type;
    AST_Type ast_type;
    int offset;
    Var *next;
};

Node *declareVariableNode(Token **cur, Type *type, Var *vars, int *cur_offset);
Node *assignNode(Token **cur, Node *var_node, Var *vars);

Var *registerVar(Var *var, char *name, Type *type, AST_Type ast_type, int offset);
Var *findVar(Var *vars, char *expectedName);

#endif