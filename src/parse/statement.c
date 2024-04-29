#include "statement.h"

Node *statementNode_(Token **cur, int cur_offset, Var *vars);

Node *statementNode(Token **cur) {
    Var *localVars = calloc(1, sizeof(Var));
    int offset = 0;
    return statementNode_(cur, offset, localVars);
}

Node *statementNode_(Token **cur, int cur_offset, Var *vars) {
    if ((*cur)->kind == EOT) return NULL;

    Node *stmt = createNode(AST_STATEMENT);

    //if ((Type *type = isModifier(&cur))) {

    Type *type = NULL;
    if ((type = isType(*cur))) {
        // int i or int i = 0
        stmt->left = declareVariableNode(cur, type, vars, &cur_offset);

    } else if ((*cur)->kind == IDENTIFIER) {
        // i = 0
        if ((*cur)->next->kind == ASSIGN) stmt->left = assignNode(cur, variableNode(cur, vars));
     
    }

    expect(*cur, SEMICOLON);
    consume(cur); // SEMICOLON

    stmt->right = statementNode_(cur, cur_offset, vars);

    return stmt;
}