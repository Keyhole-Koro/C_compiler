#include "statement.h"

void assignGen(Node *left, Node *right, int offset);

void stmtGen(Node *root) {
    if (root->type != AST_STATEMENT) {
        DEBUG_PRINT("not statement\n");
        exit(1);
    }
    int offset = 0;

    for (Node *stmt = root; stmt; stmt = stmt->right) {
        switch (stmt->type) {
            case AST_DECLARE_VAR:
                declareGen();
            case AST_ASSIGN:
                assignGen(stmt->left, &offset);
        }
    }
}

void assignGen(Node *left, Node *right, int offset) {
    //exprGen(right);

    if (left->type != AST_VARIABLE) {
        DEBUG_PRINT("not identifier");
        exit(1);
    }

    if (left->type != AST_IDENTIFIER) {
        DEBUG_PRINT("not identifier");
        exit(1);
    }

    printf("    mov dword ptr [rbp - %d], eax\n", offset);
}