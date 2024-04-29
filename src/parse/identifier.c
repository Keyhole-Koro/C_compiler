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


    DEBUG_PRINT("111\n");
    Var *registeredVar = registerVar(vars, variableName, type, ((*cur_offset) += type->size));
    // if variable exist duplicated variable, exit(1)
    DEBUG_PRINT("112\n");
    
    Node *declare = createNode(AST_DECLARE_VAR);
    consume(cur); // data type


    DEBUG_PRINT("113\n");
    Node *var = variableNode(cur, registeredVar);
    DEBUG_PRINT("114\n");

    declare->left = var;
    if ((*cur)->kind == ASSIGN) declare->right = assignNode(cur, var);
    return declare;
}

Node *assignNode(Token **cur, Node *left) {
        
    Node *assign = createNode(AST_ASSIGN);
    consume(cur);

    assign->left = left;
    assign->right = exprNode(cur);

    return assign;
}

Var *registerVar(Var *var, char *name, Type *type, int offset) {
    Var *newVar = malloc(sizeof(Var));
    newVar->name = name;
    newVar->type = type;
    newVar->offset = offset;
    newVar->next = NULL;

    DEBUG_PRINT("start %s\n", var->name);
    while (var) {
        DEBUG_PRINT("next %s\n", var->name);
        var = var->next;
    }
    var = newVar;

    return newVar;
}

bool isReservedWord(char *name) {
    return false;//temporary
}

Var *findVar(Var *vars, char *expectedName) {
    for (Var *cur_var = vars; cur_var; cur_var = cur_var->next) {
        if (cur_var->name == expectedName) return cur_var;
    }
    return NULL;
}