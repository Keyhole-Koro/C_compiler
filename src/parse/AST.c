#include "AST.h"

Node *createNode(AST_Type type, char *val) {
    Node *newNode = calloc(1, sizeof(Node));
    newNode->type = type;
    if (type == AST_NUMBER) newNode->num = val + '0';
    return newNode;
}