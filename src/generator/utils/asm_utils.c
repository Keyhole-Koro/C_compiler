#include "asm_utils.h"

Node *getFuncType(Node *func_details) {
    expectNode(func_details, AST_FUNCTION_DETAILS);

    Node *result = func_details->left;
    Node *type = result->right->right;
    return type;
}

int getVarSize(Node *var) {
    expectNode(var, AST_VARIABLE);
    Node *type = var->right;
    return type->left->value.natural;
}
int getVarOffset(Node *var) {
    expectNode(var, AST_VARIABLE);
    return var->left->value.natural;
}