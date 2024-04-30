#include "asm_utils.h"

char *asm_word = "word";
char *asm_dword = "dword";
char *asm_qword = "qword";

// for debugging make this macro later
void expectNode(Node *node, AST_Type expectedType) {
    if (node->type != expectedType) {
        DEBUG_PRINT("Unexpected Node");
        exit(1);
    }
}

char *getWord(int size) {
    switch (size) {
        case 2:
            return asm_word;
        case 4:
            return asm_dword;
        case 8:
            return asm_qword;
        default:
            DEBUG_PRINT("out of size");
            exit(1);
    }
}

int getVarSize(Node *var) {
    expectNode(var, AST_VARIABLE);
    Node *type = var->right;
    return type->left->value.natural;
}
int getVarOffset(Node *var) {
    expectNode(var, AST_VARIABLE);
    return var->left->value.natural;
}