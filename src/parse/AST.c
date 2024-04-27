#include "AST.h"

Node *createNode(AST_Type type, char *val) {
    Node *newNode = calloc(1, sizeof(Node));
    newNode->type = type;
    if (type == AST_NUMBER) newNode->num = atoi(val);
    return newNode;
}

void printPreorder(Node* node, int depth, char branch) {
    if (node == NULL)
        return;
    
    DEBUG_PRINT("%*s%c-- %d\n", depth * 4, "", branch, node->num);

    printPreorder(node->left, depth + 1, '|');

    printPreorder(node->right, depth + 1, '`');
}