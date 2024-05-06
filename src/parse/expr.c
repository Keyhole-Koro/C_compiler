#include "expr.h"
// defined in function.c
Node *callFunctionNode(Token **cur, Var *vars);

// defined in expr.c
bool isCmpOperatorNode(Token *cur);


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

Node *var_func_operator(Token **cur) {
    Node *new_node_buf = NULL;
    Node *op_node = NULL;

    if ((*cur)->kind == MUL) { // *
        int num_deferenceOp = 0;
        while (*cur && (*cur)->kind == MUL) {
            num_deferenceOp++;
            consume(cur);
        }
        op_node = createNaturalNode(AST_DEREFENCE_OPERATOR, num_deferenceOp);

    } else if ((*cur)->kind == AMPERSAND) { // &
        op_node = createNode(AST_ADDRESS_OPERATOR);
        consume(cur);

    } else { // identifier without operator
        op_node = createNode(AST_NO_OPERATOR);   
    }

    expect(*cur, IDENTIFIER);

    if ((*cur)->kind == IDENTIFIER) {
        if ((*cur)->next->kind == L_PARENTHESES) {
            new_node_buf = callFunctionNode(cur, targetLocalVariable);
        } else {
            new_node_buf = variableNode(cur, targetLocalVariable);
        }
    }

    op_node->right = new_node_buf;
    return op_node;
}

Node *factorNode(Token **cur) {

    Node *new_node = NULL;
    if ((*cur)->kind == NUMBER) {
        new_node = createNaturalNode(AST_NUMBER, atoi((*cur)->value));
        consume(cur);
        
    } else if ((*cur)->kind == MUL // derefence operator *
                || (*cur)->kind == AMPERSAND // address operator &
                || (*cur)->kind == IDENTIFIER) {
        
        new_node = var_func_operator(cur);
        
    } else if ((*cur)->kind == L_PARENTHESES) {
        consume(cur);
        new_node = exprNode_(cur);
        if ((new_node && isOperator(new_node->type))
                || ((*cur) && (*cur)->kind == R_PARENTHESES)) {
            consume(cur);
            
        } else {
            DEBUG_PRINT("token mismatches %s\n", revertToken(*cur));
            exit(1);
        }

    } else if ((*cur)->kind == ADD || (*cur)->kind == SUB) {
        TokenKind op = (*cur)->kind;
        consume(cur);
        int number = atoi((*cur)->value);
        new_node = createNaturalNode(AST_NUMBER, 
            (op == ADD) ? number : -number);
        new_node->left = factorNode(cur);
        
    } else {
        DEBUG_PRINT("Unexpected token [%s]\n", revertToken(*cur));
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
        consume(cur);
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
        consume(cur);
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