#include "array.h"

void declarAarrayGen(Node *declareArray) {
    expectNode(assignArray, AST_DECLARE_VAR);

    declareGen(declareArray);
}

void assignAarrayGen(Node *assignArray) {
    expectNode(assignArray, AST_ASSIGN);
    
    Node *array = assignArray->left;

    Node *offset = array->left;

    Node *exprIndex = offset->right;

    exprGen(exprIndex);

    int typeSize = array->right->left->value.natural;

    printf("    imul rax, %d\n", typeSize);

    printf("    add rax, %d\n", offset->value.natural);

    printf("    mov rbx, rax\n");

    Node *expr = assignArray->right;

    exprGen(expr);

    printf("    mov %s [rbp - rbx], %s\n"
                ,getWord(typeSize);
                ,getAXRegister(typeSize));
}