#include "identifier.h"

#include <stdbool.h>

Var *registerVar(Var *var, char *name, Type *type, int offset);
Var *findVar(Var *vars, char *expectedName);
bool isReservedWord(char *name);

Node *variableNode(Token **cur, Var *vars) {
    expect(*cur, IDENTIFIER);

    Token *idtfr = consume(cur);

    char *variableName = idtfr->value;

    if (isReservedWord(variableName)) {
        DEBUG_PRINT("this identifier is reserved word\n");
        exit(1);
    }
    Var *foundVar = NULL;
    if (!(foundVar = findVar(vars, variableName))) {
        DEBUG_PRINT("this variable has not defined\n");
        exit(1);
    }

    Node *var = createStringNode(AST_VARIABLE, variableName);
    var->left = createNaturalNode(AST_VARIABLE_OFFSET, foundVar->offset);
    var->right = typeNode(foundVar->type);

    return var;
}

Node *declareVariableNode(Token **cur, Type *type, Var *vars, int *cur_offset) {
    /** @brief avoid deplicates*/
    expect((*cur)->next, IDENTIFIER);
    char *variableName = (*cur)->next->value;
    // consider dunctions' identifiers later
    if (findVar(vars, variableName)) {
        DEBUG_PRINT("duplicate identifier\n");
        exit(1);
    }

    Var *registeredVar = registerVar(vars, variableName, type, ((*cur_offset) += type->size));
    // if variable exist duplicated variable, exit(1)
    
    Node *declare = createNode(AST_DECLARE_VAR);
    consume(cur); // data type

    Node *var_node = variableNode(cur, registeredVar);

    declare->left = var_node;

    return declare;
}

// assign if neccesary
Node *declareAssignVariableNode(Token **cur, Type *type, Var *vars, int *cur_offset) {
    Node *declare = declareVariableNode(cur, type, vars, cur_offset);

    Node *var_node = declare->left;
    if ((*cur)->kind == ASSIGN) declare->right = assignNode(cur, var_node, vars);
    return declare;
}


Node *assignNode(Token **cur, Node *var_node, Var *vars) {
        
    Node *assign = createNode(AST_ASSIGN);
    consume(cur);

    Node *expr = createNode(AST_EXPR);
    expr->left = exprNode(cur, vars);

    assign->left = var_node;
    assign->right = expr;

    return assign;
}

bool isReservedWord(char *name) {
    return false;//temporary
}