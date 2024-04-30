#include "asm_generator.h"

#include <stdio.h>

void asm_config();
void asm_printf();
void returnSuccess();

#define ASM_HEADER "bits 64\n\n"
#define PRINT_FORMAT "    fmt:    db \"%%d\", 10, 0\n\n"
#define EXTERN_PRINTF "extern printf\n\n"

void asmGen(Node *root) {
    asm_config();
    
    while (root) {
        if (root->type == AST_PROGRAM) funcGen(root->left);
        else {
            DEBUG_PRINT("not ast program\n");
            exit(1);
        }
    } 

    asm_printf();
    returnSuccess();
}

void asm_config() {
    printf(ASM_HEADER);
    printf("section .data\n");
    printf(PRINT_FORMAT);
    printf(EXTERN_PRINTF);
    printf("section .text\n");
    printf("    global _start\n");
    printf("\n");
}

void asm_printf() {
    printf("    xor rax, rax\n");
    printf("    mov rdi, fmt\n");
    printf("    mov esi, [rbp - 4]\n");
    printf("    call printf\n");
    printf("\n");
}

void returnSuccess() {
    printf("    mov rdi, 0\n");
    printf("    mov rax, 60\n");
    printf("    syscall\n");
    printf("\n");
}