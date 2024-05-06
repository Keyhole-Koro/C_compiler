#include "pointer.h"

Node *pointerOperatorNode(Token **cur) {
    int num_deferenceOp = 0;

    while (*cur && (*cur)->kind == MUL) {
        num_deferenceOp++;
        *cur = (*cur)->next;
    }

    if (num_deferenceOp == 0) return NULL;

    return createNaturalNode(AST_POINTER, num_deferenceOp);
}

Node *getPointerNode(Node *var) {
    expectNode(var, AST_VARIABLE);
    
    Node *type = var->right;
    return type->right;
}

int getNumPointer(Node *pointer_node) {
    expectNode(pointer_node, AST_POINTER);
    return pointer_node->value.natural;
}

int getNumDerefenceOperator(Node *derefence) {
    expectNode(derefence, AST_DEREFENCE_OPERATOR);
    return derefence->value.natural;
}