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

    int offset = getVarOffset(var);
        
    char *word = getWord(getVarSize(var));

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