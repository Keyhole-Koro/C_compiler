#include "asm_conditional_expr.h"

void conditional_stmtGen(Node *condi_stmt, Label *trueBranch) {
    expect(condi_stmt, AST_CONDITIONAL_STATEMTNT);

    Node *condition = condi_stmt->left;
    expect(condition, AST_CONDITION);

    conditional_exprGen(condition, trueBranch);
}

void conditional_exprGen(Node *condi, Var *vars, Label *trueBranch) {
    if (isLogicalOperator(condi)) {
        logicalOperationGen(condi, trueBranch);
    } else if (isCmparisionOperatorNode(condi)) {
        comparisionGen(condi, trueBranch);
    } else {
        DEBUG_PRINT("Unsupported\n");
        exit(1);
    }
}

Label *logicalOperationGen(Node *logi, Label *trueBranch) {
    if (isLogicalOperator(logi->left)) {
        trueBranch = logicalOperationGen(logi->left, trueBranch);
    }
    if (isLogicalOperator(logi->right)) {
        logicalOperationGen(logi->right, trueBranch);
    }

    if (logi->type == AST_AND) {
            Label *relayBranch = makeBranchLabel();
            comparisionGen(logi->left, relayBranch, AST_AND);
            comparisionGen(logi->right, trueBranch, AST_AND);
            return relayBranch;

    } else if (logi->type == AST_OR) {
            comparisionGen(logi->left, trueBranch, AST_OR);
            comparisionGen(logi->right, trueBranch, AST_OR);
            return trueBranch;

    } else {
        DEBUG_PRINT("Unsupported\n");
        exit(1);
    }
}

void comparisionGen(Node *cmp, Label *trueBranch, AST_Type type) {
    exprGen(cmp->left);
    printf("   mov ecx, eax\n");
    exprGen(cmp->right);
    printf("   cmp ecx, eax\n");
    if (type == AST_AND) {
        printf("   %s %s\n", getJumpInstruct_signed(cmp), branch->name);
        jumpTo();
    }
}

bool isLogicalOperator(Node *node) {
    switch (node->type) {
        case AND:
        case OR:
            return true;
        default:
            return false;
    }
}

bool isCmparisionOperatorNode(Node *node) {
    switch (node->type) {
        case EQ:
        case NEQ:
        case LT:
        case LTE:
        case GT:
        case GTE:
            return true;
        default:
            return false;
    }
}