#ifndef VARIABLE_H
#define VARIABLE_H

#include "type.h"

typedef struct Var Var;

struct Var {
    char *name;
    Type *type;
    int offset;
    Var *next;
};

Var *registerVar(Var *var, char *name, Type *type, int offset);
Var *findVar(Var *vars, char *expectedName);

#endif