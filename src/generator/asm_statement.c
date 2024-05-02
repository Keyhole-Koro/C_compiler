#include "asm_statement.h"

// defined in asm_funciton.c
void callFuncGen(Node *callFunc);


void stmtGen(Node *root) {
    if (root->type != AST_STATEMENT) {
        DEBUG_PRINT("not statement\n");
        exit(1);
    }

    for (Node *stmt = root; stmt; stmt = stmt->right) {
        switch (stmt->left->type) {
            case AST_DECLARE_VAR:
                declareGen(stmt->left);
                break;
            case AST_ASSIGN:
                assignGen(stmt->left);
                break;
            case AST_CALL_FUNC:
                callFuncGen(stmt->left);
            default:
                DEBUG_PRINT("out of definition\n");
                break;
        }
    }
}

