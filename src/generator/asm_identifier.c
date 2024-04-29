#include "identifier.h"

void assignGen(Node *assign, int offset) {
    //exprGen(right);

    if (assign->left->type != AST_VARIABLE) {
        DEBUG_PRINT("not identifier");
        exit(1);
    }

    if (assign->right->type != AST_EXPR) {
        DEBUG_PRINT("not identifier");
        exit(1);
    }

    printf("    mov dword ptr [rbp - %d], eax\n", offset);
}