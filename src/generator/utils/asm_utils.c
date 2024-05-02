#include "asm_utils.h"

char *asm_byte = "byte";
char *asm_word = "word";
char *asm_dword = "dword";
char *asm_qword = "qword";

char *asm_di = "rdi\0edi\0di";
char *asm_si = "rsi\0esi\0si";
char *asm_dx = "rdx\0edx\0dx";
char *asm_cx = "rcx\0ecx\0cx";
char *asm_r8 = "r8";
char *asm_r9 = "r9";


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

    char *regi = NULL;
    switch (nth) {
        case 1:
            regi = asm_di;
            break;
        case 2:
            regi = asm_si;
            break;
        case 3:
            regi = asm_dx;
            break;
        case 4:
            regi = asm_cx;
            break;
        case 5:
            return asm_r8;
        case 6:
            return asm_r9;
        default:
            DEBUG_PRINT("out of size\n");
            exit(1);
    }

    switch (size) {
        case 1:
            DEBUG_PRINT("hasnt prepared yet\n");
            exit(1);
        case 2:
            return regi + 4;
        case 4:
            return regi + 4;
        case 8:
            return regi;
        default:
            DEBUG_PRINT("out of size");
            exit(1);
    }

    return NULL;
}

Node *getFuncType(Node *func_details) {
    expectNode(func_details, AST_FUNCTION_DETAILS);

    Node *result = func_details->left;
    Node *type = result->right->right;
    return type;
}