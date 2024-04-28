#include <stdio.h>
#include "asm_generator.h"

void exprGen(Node *root);

void num_to_string();

void asmGen(Node *root) {
    printf("bits 64\n");

    printf("\n");

    printf("section .data\n");
    printf("    fmt:    db \"%%d\", 10, 0\n");
    
    printf("\n");

    printf("extern printf\n");

    //printf("section .bss\n");
    //printf("    num_buffer: resb 16\n"); 
    printf("\n");

    printf("section .text\n");
    printf("    global _start\n");
    printf("\n");

    printf("_start:\n");

    exprGen(root);

    //printf("    mov rdi, num_buffer\n");
    //printf("    call num_to_string\n");

    printf("\n");

    printf("    mov rdi, fmt\n");
    printf("    mov esi, eax\n");
    printf("    call printf\n");
    printf("\n");

    printf("    mov rdi, 0\n");
    printf("    mov rax, 60\n");
    printf("    syscall\n");

    //num_to_string();
}

void num_to_string() {
    printf("\n");
    printf("num_to_string:\n");
    printf("    push rbx\n");
    printf("    mov rbx, 10\n");
    printf("    mov rsi, rdi\n");
    printf("    mov rdi, num_buffer\n");
    printf("    mov rcx, 0\n");
    // Implementation of num_to_string here
    printf("    pop rbx\n");
    printf("    ret\n");
}

void exprGen(Node *root) {
    if (!root) return;

    if (root->type == AST_NUMBER) {
        printf("    mov eax, %d\n", root->num);
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
                printf("    ; Unknown node type\n");
                break;
        }
    }
}
