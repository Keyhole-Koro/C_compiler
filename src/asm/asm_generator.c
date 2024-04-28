#include <stdio.h>
#include "asm_generator.h"

void exprGen(Node *root);

#define ASM_HEADER "bits 64\n\n"
#define PRINT_FORMAT "fmt:    db \"%%d\", 10, 0\n\n"
#define EXTERN_PRINTF "extern printf\n\n"

void asm_config() {
    printf(ASM_HEADER);
    printf(PRINT_FORMAT);
    printf(EXTERN_PRINTF);
    printf("section .text\n");
    printf("    global _start\n\n");
}

void asm_printf() {
    printf("    mov rdi, fmt\n");
    printf("    mov esi, eax\n");
    printf("    call printf\n\n");
}

void returnSuccess() {
    printf("    mov rdi, 0\n");
    printf("    mov rax, 60\n");
    printf("    syscall\n\n");
}

void asmGen(Node *root) {
    asm_config();
    printf("_start:\n");
    exprGen(root);
    asm_printf();
    returnSuccess();
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
