#ifndef AST_H
#define AST_H

#include <stdlib.h>

#include "token.h"

#include "debug.h"

typedef enum {
    AST_NUMBER,

    AST_ADD,
    AST_SUB,
    AST_MUL,
    AST_DIV,

    AST_EXPR,

    AST_STATEMENT,

    AST_DECLARE_VAR,
    AST_IDENTIFIER,
    AST_VARIABLE,
    AST_VARIABLE_OFFSET,
    AST_TYPE,
    AST_TYPE_SIZE,

    AST_ASSIGN,

} AST_Type;

typedef enum {
    NATURAL_NUMBER,
    DECIMAL_FRACTION,
    STRING,
    NOTHING
} ValueKind;

typedef union Value Value;
union Value {
    char *str;
    int natural;
    float decimal;
};

typedef struct Node Node;
struct Node{
    AST_Type type;
    ValueKind valKind;
    Value value;
    Node *left;
    Node *right;
};

Node *createNode(AST_Type type);

Node *createNaturalNode(AST_Type type, int value);
Node *createDecimalNode(AST_Type type, float value);
Node *createStringNode(AST_Type type, char *value);

void printPreorder(struct Node* node, int depth, char branch);

#endif