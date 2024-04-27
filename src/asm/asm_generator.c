#include "asm_generator.h"

void exprGen(Node *root);

void asmGen(Node *root) {
    printf("bits 64\n");
    printf("section .text\n");
    printf("global main\n");
    printf("\n");

    printf("main:\n");

    exprGen(root);

    printf("\n");

    printf("    mov [result], rax\n");

    printf("\n");

    printf("    mov rsi, result\n");
    printf("    mov rdx, 8\n");
    printf("    mov rdi, 1\n");
    printf("    mov rax, 1\n");
    printf("    syscall\n");
    printf("\n");

    

    printf("    mov rdi, 0\n");
    printf("    mov rax, 60\n");
    printf("    syscall\n");

    //printf("    ret\n");

    printf("\n");

    printf("section .data\n");
    printf("result dq 0\n");

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
                DEBUG_PRINT("Unknown node type\n");
                break;
        }
    }
}
