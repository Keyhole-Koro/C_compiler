#include "asm_identifier.h"

void assignGen(Node *assign) {

    Node *var = assign->left;
    if (var->type != AST_VARIABLE) {
        DEBUG_PRINT("not identifier");
        exit(1);
    }

    Node *expr = assign->right;
    if (expr->type != AST_EXPR) {
        DEBUG_PRINT("not identifier");
        exit(1);
    }

    exprGen(expr->left);

    int offset = var->left->value.natural;
    Node *type = var->right;

    int size = type->left->value.natural;
    char *word = NULL;
    
    switch (size) {
        case 2:
            word = "word";
            break;
        case 4:
            word = "dword";
            break;
        case 8:
            word = "qword";
            break;
        default:
            DEBUG_PRINT("out of size");
            exit(1);
    }

    printf("    mov %s [rbp - %d], eax\n", word, offset);
}

void declareGen(Node *declare) {
    Node *var = declare->left;
    if (var->type != AST_VARIABLE) {
        DEBUG_PRINT("not identifier");
        exit(1);
    }

    if (declare->right->type == AST_ASSIGN) assignGen(declare->right);
}