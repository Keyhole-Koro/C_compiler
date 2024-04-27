#include "asm_generator.h"

void exprGen(Node *root);

void asmGen(Node *root) {
    printf("bits 64\n");
    printf("section .text\n");
    printf("global _start\n");
    printf("extern write, exit\n");
    printf("_start:\n");
    printf("\n");

    exprGen(root);

    printf("\n");

    printf("    mov rsi, rax\n");
    printf("    mov rdx, 14\n");
    printf("    mov rdi, 1\n");
    printf("    mov rax, 1\n");
    printf("    syscall\n");

    printf("    ret\n");

    printf("\n");

    printf("section .data\n");
    printf("result_format db \"Result: %%d\", 10, 0\n");

}

void exprGen(Node *root) {
    if (!root) return;

    if (root->type == AST_NUMBER) {
        printf("    mov eax, %d\n", root->num);
    } else {
        exprGen(root->left);
        printf("    push rax\n");
        exprGen(root->right);
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
                fprintf(stderr, "Unknown node type\n");
                break;
        }
    }
}
