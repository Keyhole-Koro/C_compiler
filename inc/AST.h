#ifndef AST_H
#define AST_H

#include "token.h"

typedef enum {
    AST_NUMBER,

    AST_ADD,
    AST_SUB,
    AST_MUL,
    AST_DEV,
} AST_Type;

typedef struct {
    AST_Type type;
    int num;
    char *val;
    struct Node *left;
    struct Node *right;
} Node;

Node *createNode(AST_Type type, char *val);

#endif