#include "statement.h"

Node *statementNode(Token **cur, int *cur_offset, Var *vars) {
    if ((*cur)->kind == EOT
        || (*cur)->kind == R_BRACE
        || (*cur)->kind == RETURN) return NULL;

    Node *stmt = createNode(AST_STATEMENT);

    //if ((Type *type = isModifier(&cur))) {

    Type *type = NULL;
    if ((type = isType(*cur))) {
        // int i or int i = 0
        stmt->left = declareAssignVariableNode(cur, type, vars, cur_offset);

    } else if ((*cur)->kind == IDENTIFIER) {
        // i = 0
        if ((*cur)->next->kind == ASSIGN) stmt->left = assignNode(cur, variableNode(cur, vars), vars);
     
    }

    expect(*cur, SEMICOLON);
    consume(cur); // SEMICOLON

    stmt->right = statementNode(cur, cur_offset, vars);

    return stmt;
}