#include "identifier.h"

#include <stdbool.h>

Var *findVar(Var *vars, char *expectedName);

Node *variableNode(Token **cur, Var *vars) {
    expect(*cur, IDENTIFIER);
    Token *idtfr = consume(cur);

    char *varName = idtfr->value;

    if (isReservedWord(varName)) {
        DEBUG_PRINT("this identifier is a reserved word\n");
        exit(1);
    }
    Var *foundVar = NULL;
    if (!(foundVar = findVar(vars, varName))) {
        DEBUG_PRINT("this variable has not defined\n");
        exit(1);
    }

    Node *var = createStringNode(AST_VARIABLE, varName);
    // take this line carefully when you add syntax error handler
    // if foundVar was not found, pass this part otherwise, segmentation fault
    // foundVar->type
    var->left = createNaturalNode(AST_VARIABLE_OFFSET, foundVar->offset);
    var->right = typeNode(foundVar->type);

    return var;
}

Node *declareVariableNode(Token **cur, Type *type, Var *vars, int *cur_offset) {
    if (!isType(*cur)
        && !((*cur)->kind == IDENTIFIER)) {
        DEBUG_PRINT("Unexpected token\n");
        exit(1);
    }

    Node *declare = createNode(AST_DECLARE_VAR);
    consume(cur); // data type

    Node *pointer = pointerOperatorNode(cur);

    if (pointer && getNumPointer(pointer) > 0) type->size = 8;
 
    /** @brief avoid deplicates*/
    expect(*cur, IDENTIFIER);
    char *varName = (*cur)->value;
    // consider dunctions' identifiers later
    if (findVar(vars, varName)) {
        DEBUG_PRINT("duplicate identifier\n");
        exit(1);
    }

    Var *registeredVar = NULL;
    // if variable exist duplicated variable, exit(1)
    
    Node *var_node = NULL;
    
    // cur is IDENTIFIER currently
    if ((*cur)->next->kind == L_BRACKET) {       
        registeredVar = registerVar(vars, varName, type, AST_ARRAY, ((*cur_offset) += type->size));
        var_node = arrayNode(cur, registeredVar, cur_offset);
    } else {
        registeredVar = registerVar(vars, varName, type, AST_VARIABLE, ((*cur_offset) += type->size));
        var_node = variableNode(cur, registeredVar);
    }
    
    
    Node *type_node = var_node->right;
    if (pointer) type_node->right = pointer;
    
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

    int index = -1;

    if ((*cur)->kind == L_BRACKET) {
        consume(cur);

        Node *expr = exprNode(cur, vars);

        var_node->left->right = expr;

        expect(*cur, R_BRACKET);
        consume(cur);
    }

    Node *expr = createNode(AST_EXPR);
    expr->left = exprNode(cur, vars);

    assign->left = var_node;
    assign->right = expr;

    return assign;
}

bool isReservedWord(char *name) {
    return false; //temporary
}