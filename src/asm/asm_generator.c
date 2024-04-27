#include "asm_generator.h"

void asmGen(Node *root) {
    printf("bits 64\n");
    printf("section .text\n");
    printf("global main\n");
    printf("main:\n");

    exprGen(root);
}

void exprGen(Node *root){
    if (!root) return;

    if (root->type == AST_NUMBER) {
        printf("    mov eax, %d\n", root->value);
    } else {
        exprGen(root->left);
        printf("    push eax\n");
        exprGen(root->right);
        printf("    pop ebx\n");
        
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
                printf("    idiv ebx\n");
                break;
            default:
                fprintf(stderr, "Unknown node type\n");
                break;
        }
    }
}