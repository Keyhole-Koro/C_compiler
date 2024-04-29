#include "asm_expr.h"

void exprGen(Node *root) {
    if (!root) return;

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
            default:
                DEBUG_PRINT("Unknown node type\n");
                break;
        }
    }
}
