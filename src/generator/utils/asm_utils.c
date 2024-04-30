#include "asm_utils.h"

char *asm_byte = "byte";
char *asm_word = "word";
char *asm_dword = "dword";
char *asm_qword = "qword";

char *asm_di = "di";
char *asm_si = "si";
char *asm_dx = "dx";
char *asm_cx = "cx";
char *asm_r8 = "r8";
char *asm_r9 = "r9";

// for debugging make this macro later
void expectNode(Node *node, AST_Type expectedType) {
    if (node->type != expectedType) {
        DEBUG_PRINT("Unexpected Node");
        exit(1);
    }
}

char *getWord(int size) {
    switch (size) {
        case 1:
            return asm_byte;
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

char *getParamRegister(int nth, int size) {
    if (nth > 6) {
        DEBUG_PRINT("hasnt prepared yet\n");
        exit(1);
    }

    char *suffix = NULL;
    switch (nth) {
        case 1:
            suffix = asm_di;
            break;
        case 2:
            suffix = asm_si;
            break;
        case 3:
            suffix = asm_dx;
            break;
        case 4:
            suffix = asm_cx;
            break;
        case 5:
            return r8;
        case 6:
            return r9;
        default:
            DEBUG_PRINT("out of size\n");
            exit(1);
    }

    char *prefix = NULL;
    char *combined = NULL;
    switch (size) {
        case 1:
            DEBUG_PRINT("hasnt prepared yet\n");
            exit(1);
        case 2:
            return suffix;
        case 4:
            prefix = "e";
            break;
        case 8:
            prefix = "r";
            break;
        default:
            DEBUG_PRINT("out of size");
            exit(1);
    }

    combined = malloc(strlen(prefix) + strlen(suffix));
    strcpy(combined, prefix);
    strcat(combined, suffix);

    return combined;
}