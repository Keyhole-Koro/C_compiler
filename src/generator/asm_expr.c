#include "asm_expr.h"

// deined in asm_funciton.c
void callFuncGen(Node *callFunc);

// defined in asm_funciton.c
int getReturnSize(Node *func);


void exprGen(Node *root) {
    if (!root) return;

    switch (root->type) {
        case AST_ADD:
        case AST_SUB:
        case AST_MUL:
        case AST_DIV:
        case AST_NO_OPERATOR:
        case AST_DEREFENCE_OPERATOR:
        case AST_ADDRESS_OPERATOR:
        case AST_NUMBER:
            break;
        default:
            return;
    }

    Node *node_buf = NULL;

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
            
            case AST_DEREFENCE_OPERATOR:
                node_buf = root->right;
                switch(node_buf->type) {
                    case AST_VARIABLE:
                        printf("    mov rax, qword [rbp - %d]\n"
                                ,getVarOffset(node_buf));

                        for (int i = 0; i < getNumDerefenceOperator(root) - 1; i++) {
                            printf("    mov rax, qword [rax]\n");
                        }
                        printf("    mov %s, %s [rax]\n"
                                ,getAXRegister(getVarSize(node_buf))
                                ,getWord(getVarSize(node_buf)));
                        break;
                    case AST_CALL_FUNC:
                        callFuncGen(node_buf);
                        for (int i = 0; i < getNumDerefenceOperator(root) - 1; i++) {
                            printf("    mov rax, qword [rax]\n");
                        }
                        printf("    mov %s, %s [rax]\n"
                                ,getAXRegister(getReturnSize(node_buf))
                                ,getWord(getReturnSize(node_buf)));
                        break;
                    default:
                        DEBUG_PRINT("Unxpected node\n");
                        exit(1);
                }
                break;

            case AST_ADDRESS_OPERATOR:
                node_buf = root->right;
                switch(node_buf->type) {
                    case AST_VARIABLE:
                        printf("    lea rax, qword [rbp - %d]\n"
                                ,getVarOffset(node_buf));
                        break;
                    case AST_CALL_FUNC:
                        callFuncGen(node_buf);
                        printf("    lea rax, %s\n"
                                ,getAXRegister(getReturnSize(node_buf)));
                        break;
                    default:
                        DEBUG_PRINT("Unxpected node\n");
                        exit(1);
                }
                break;

            case AST_NO_OPERATOR:
                node_buf = root->right;
                switch(node_buf->type) {
                    case AST_VARIABLE:
                        printf("    mov %s, %s [rbp - %d]\n"
                                ,getAXRegister(getVarSize(node_buf))
                                ,getWord(getVarSize(node_buf))
                                ,getVarOffset(node_buf));
                        break;
                    case AST_CALL_FUNC:
                        callFuncGen(node_buf);
                        break;
                    default:
                        DEBUG_PRINT("Unxpected node\n");
                        exit(1);
                }
                break;
            default:
                DEBUG_PRINT("Unexpected Type [%d\n", root->type);
                break;
            
        }
    }
}