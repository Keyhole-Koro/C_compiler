#ifndef AST_H
#define AST_H

#include <stdlib.h>

#include "token.h"

typedef enum {
    AST_NUMBER,

    AST_ADD,
    AST_SUB,
    AST_MUL,
    AST_DIV,
} AST_Type;

typedef struct Node Node;

struct Node{
    AST_Type type;
    int num;
    char *val;
    Node *left;
    Node *right;
} ;

Node *createNode(AST_Type type, char *val);

void printPreorder(struct Node* node, int depth, char branch);

#endif