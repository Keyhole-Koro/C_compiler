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
    
    printf("    mov %s [rbp - %d], %s\n"
            ,getWord(getVarSize(var))
            ,getVarOffset(var)
            ,getAXRegister(getVarSize(var)));
    
}

void declareGen(Node *declare) {
    Node *var = declare->left;
    if (var->type != AST_VARIABLE) {
        DEBUG_PRINT("not identifier");
        exit(1);
    }

    if (declare->right->type == AST_ASSIGN) assignGen(declare->right);
}