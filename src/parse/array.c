#include "array.h"

// defined in identifier.c
bool isReservedWord(char *name);

Node *arrayNode(Token **cur, Var *vars, int *cur_offset) {
    expect(*cur, IDENTIFIER);
    Token *idfr = consume(cur);

    char *varName = idfr->value;

    if (isReservedWord(varName)) {
        DEBUG_PRINT("this identifier is a reserved word\n");
        exit(1);
    }
    Var *foundVar = NULL;
    if (!(foundVar = findVar(vars, varName))) {
        DEBUG_PRINT("this variable has not defined\n");
        exit(1);
    }

    expect(*cur, L_BRACKET);
    consume(cur);

    // expr(cur, vars);

    expect(*cur, NUMBER);
    Token *numTk = consume(cur);
    int index = atoi(numTk->value);

    expect(*cur, R_BRACKET);
    consume(cur);

    Node *offset = createNaturalNode(AST_VARIABLE_OFFSET, foundVar->offset);
    offset->left = createNaturalNode(AST_INDEX, index);
    
    Node *array = createStringNode(AST_ARRAY, varName);
    array->left = offset;
    array->right = typeNode(foundVar->type);

    *cur_offset += foundVar->type->size * index;

    return array;
}