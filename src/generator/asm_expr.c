#include "asm_expr.h"

// deined in asm_funciton.c
void callFuncGen(Node *callFunc);


void exprGen(Node *root) {
    if (!root) return;

    switch (root->type) {
        case AST_ADD:
        case AST_SUB:
        case AST_MUL:
        case AST_DIV:
        case AST_NUMBER:
        case AST_VARIABLE:
        case AST_CALL_FUNC:
            break;
        default:
            return;
    }

    if (root->type == AST_NUMBER) {
        printf("    mov eax, %d\n", root->value.natural);
    } else {
        exprGen(root->right);
        printf("    push rax\n");
        exprGen(root->left);
        printf("    pop rbx\n");
        
        switch (root->type) {
            case AST_ADD:
                printf("    add eax, ebx\n");
                break;
            case AST_SUB:
                printf("    sub eax, ebx\n");
                break;
            case AST_MUL:
                printf("    imul eax, ebx\n");
                break;
            case AST_DIV:
                printf("    mov edx, 0\n");
                printf("    idiv ebx\n");
                break;
            case AST_VARIABLE:
                printf("    mov eax, %s [rbp - %d]\n", getWord(getVarSize(root)), getVarOffset(root));
                break;
            case AST_CALL_FUNC:
                callFuncGen(root);
                break;
            default:
                DEBUG_PRINT("Unexpected Type [%d]\n", root->type);
                break;
        }
    }
}

