#include "expr.h"

#include <stdbool.h>

/** 
    @brief specifies the target variable list
        to extract the infomation of the variable offset, size, etc

*/

Var *targetLocalVariable = NULL;

Node *factorNode(Token **cur);
Node *termNode(Token **cur);
Node *exprNode_(Token **cur);

bool isOperator(AST_Type type) {
    switch (type){
        case AST_ADD:
        case AST_SUB:
        case AST_MUL:
        case AST_DIV:
            return true;
        default:
            return false;
    }
}

Node *factorNode(Token **cur) {
    Node *new_node = NULL;
    if ((*cur)->kind == NUMBER) {
        new_node = createNaturalNode(AST_NUMBER, atoi((*cur)->value));
        *cur = (*cur)->next;
    } else if ((*cur)->kind == IDENTIFIER) {
        DEBUG_PRINT("IDENTIFIER\n");
        new_node = variableNode(cur, targetLocalVariable);
    } else if ((*cur)->kind == L_PARENTHESES) {
        *cur = (*cur)->next;
        new_node = exprNode_(cur);
        if ((new_node && isOperator(new_node->type))
                || ((*cur) && (*cur)->kind == R_PARENTHESES)) {
            *cur = (*cur)->next;
            return new_node;
        } else {
            DEBUG_PRINT("token mismatches\n");
            exit(1);
        }
    } else if ((*cur)->kind == ADD || (*cur)->kind == SUB) {
        TokenKind op = (*cur)->kind;
        *cur = (*cur)->next;
        int number = atoi((*cur)->value);
        new_node = createNaturalNode(AST_NUMBER, 
            (op == ADD) ? number : -number);
        new_node->left = factorNode(cur);
    } else {
        DEBUG_PRINT("Unexpected token\n");
        exit(1);
    }
    return new_node;
}


Node *termNode(Token **cur) {
    if (!(*cur)) return NULL;

    Node *new_node = NULL;

    new_node = factorNode(cur);

    while (*cur && ((*cur)->kind == MUL || (*cur)->kind == DIV)) {
        Token *op = *cur;
        *cur = (*cur)->next;
        Node *op_node = createNode((op->kind == MUL) ? AST_MUL : AST_DIV);
        op_node->left = new_node;
        op_node->right = factorNode(cur);
        new_node = op_node;
    }

    return new_node;
}

Node *exprNode_(Token **cur) {
    if (!(*cur)) return NULL;

    Node *new_node = termNode(cur);

    while (*cur && ((*cur)->kind == ADD || (*cur)->kind == SUB)) {
        Token *op = *cur;
        *cur = (*cur)->next;
        Node *op_node = createNode((op->kind == ADD) ? AST_ADD : AST_SUB);
        op_node->left = new_node;
        op_node->right = termNode(cur);
        new_node = op_node;
    }

    return new_node;
}

Node *exprNode(Token **cur, Var *targetVar) {
    targetLocalVariable = targetVar;
    return exprNode_(cur);
}