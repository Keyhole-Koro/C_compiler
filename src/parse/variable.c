#include "variable.h"

Var *registerVar(Var *var, char *name, Type *type, AST_Type ast_type, int offset) {
    Var *newVar = malloc(sizeof(Var));
    newVar->name = name;
    newVar->type = type;
    newVar->ast_type = ast_type;
    newVar->offset = offset;
    newVar->next = NULL;

    while (var->next) {
        var = var->next;
    }
    var->next = newVar;

    return newVar;
}

Var *findVar(Var *vars, char *expectedName) {
    for (Var *cur_var = vars; cur_var; cur_var = cur_var->next) {
        if (!cur_var->name) continue;
        if (strcmp(cur_var->name, expectedName) == 0) return cur_var;
    }
    return NULL;
}