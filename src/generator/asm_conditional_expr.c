#include "asm_conditional_expr.h"

#include "asm_conditional_expr.h"

void conditoinalExprGen(Node *condi_stmt, Label *trueBranch, Label *falseBranch);

Label *logicalOperationGen(Node *logi, Label *trueBranch, Label *falseBranch);
void comparisionGen(Node *cmp, Label *trueBranch);
bool isTypeLogicalOperator(Node *node);
bool isTypeCmparisionOperator(Node *node);

void conditionGen(Node *condi_stmt, Label *trueBranch, Label *falseBranch) {
    expectNode(condi_stmt, AST_CONDITIONAL_STATEMTNT);
    
    Node *condition = condi_stmt->left;

    conditoinalExprGen(condition, trueBranch, falseBranch);
}

void conditoinalExprGen(Node *condition, Label *trueBranch, Label *falseBranch) {
    
    if (isTypeLogicalOperator(condition)) {
        trueBranch = logicalOperationGen(condition, trueBranch, falseBranch);
    } else if (isTypeCmparisionOperator(condition)) {
        comparisionGen(condition, trueBranch);

    } else {
        DEBUG_PRINT("Unexpected node\n");
        exit(1);
    }
}

Label *logicalOperationGen(Node *logi, Label *trueBranch, Label *falseBranch) {

    if (logi->type == AST_AND)
    { 
        Label *relayBranch = makeRelayLabel();
        conditoinalExprGen(logi->left, relayBranch, falseBranch);
        jumpTo(falseBranch);
        startLabel(relayBranch);
        conditoinalExprGen(logi->right, trueBranch, falseBranch);
    }
    else if (logi->type == AST_OR) {
        conditoinalExprGen(logi->left, trueBranch, falseBranch);
        conditoinalExprGen(logi->right, trueBranch, falseBranch);
    }
    else {
        DEBUG_PRINT("Unsupported\n");
        exit(1);
    }
    

    return trueBranch;
}

void comparisionGen(Node *cmp, Label *trueBranch) {
    exprGen(cmp->left);
    printf("    mov ecx, eax\n");
    exprGen(cmp->right);
    printf("    cmp ecx, eax\n");
    printf("    %s %s\n", getJumpInstruct_signed(cmp), trueBranch->name);
}

bool isTypeLogicalOperator(Node *node) {
    switch (node->type) {
        case AST_AND:
        case AST_OR:
            return true;
        default:
            return false;
    }
}

bool isTypeCmparisionOperator(Node *node) {
    switch (node->type) {
        case AST_EQ:
        case AST_NEQ:
        case AST_LT:
        case AST_LTE:
        case AST_GT:
        case AST_GTE:
            return true;
        default:
            return false;
    }
}