#include "if.h"

// defined in statement.c
Node *statementNode(Token **cur, int *cur_offset, Var *vars);

Node *ifNode(Token **cur, int *cur_offset, Var *vars) {

    Node *if_node = NULL;
    switch ((*cur)->kind) {
        case IF:
            consume(cur);
            if_node = createNode(AST_IF);
            break;
        case ELSE:
            consume(cur);
            if ((*cur)->kind == IF) {
                if_node = createNode(AST_ELSE_IF);
                consume(cur);
                break;
            }
            if_node = createNode(AST_ELSE);
            break;
        default:
            DEBUG_PRINT("out of senario\n");
            exit(1);
    }

    Node *condi_stmt = createNode(AST_CONDITIONAL_STATEMTNT);

    if_node->left = condi_stmt;

    if (if_node->type != AST_ELSE) {
        // '(' expr ')'

        expect(*cur, L_PARENTHESES);
        consume(cur);
        
        condi_stmt->left = condtionalExprNode(cur, vars);

        expect(*cur, R_PARENTHESES);
        consume(cur);
    }
    // support a line stmt later
    expect(*cur, L_BRACE);
    consume(cur);

    condi_stmt->right = statementNode(cur, cur_offset, vars);

    expect(*cur, R_BRACE);
    consume(cur);

    if ((*cur)->kind == ELSE && if_node->type != AST_ELSE) if_node->right = ifNode(cur, cur_offset, vars); 

    return if_node;
}