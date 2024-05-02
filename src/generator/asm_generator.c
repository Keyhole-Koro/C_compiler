#include "asm_generator.h"

#include <stdio.h>

void asm_config();

void returnSuccess();

#define ASM_HEADER "bits 64\n\n"
#define PRINT_FORMAT "    fmt:    db \"%%d\", 10, 0\n\n"
#define EXTERN_PRINTF "extern printf\n\n"

void asmGen(Node *root) {
    asm_config();
    
    while (root && root->type == AST_PROGRAM) {
        if (root->left 
            && root->left->type == AST_FUNCTION) funcGen(root->left);
        root = root->right;
    } 
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