#ifndef FUNCTION_H
#define FUNCTION_H

#include "token.h"
#include "AST.h"

#include "debug.h"

#include "type.h"
#include "expr.h"
#include "identifier.h"
#include "statement.h"


typedef struct Func Func;

struct Func {
    char *name;
    Type *type;
    Node *func_details;
    Func *next;
};

extern Func *head_functions;

Node *functionNode(Token **cur);

#endif