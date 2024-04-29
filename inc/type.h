#ifndef TYPE_H
#define TYPE_H

#include "AST.h"

typedef struct {
    char *name;
    int size;
} Type;

Type *isType(Token *tk);

Node *typeNode(Type *type);

#endif