#include "conditional_expr.h"

Node *comparisionOperatorNode(Token **cur);
Node *logicalOperatorNode(Token **cur, Var *vars);

bool isLogicalOperator(Token *tk);
bool isLogicalOperatorInExpr(Token *cur);

// && ||
Node *logicalOperatorNode(Token **cur, Var *vars) {

    Node *logi = NULL;
    switch ((*cur)->kind) {
        case AND:
            logi = createNode(AST_AND);
            break;
        case OR:
            logi = createNode(AST_OR);
            break;
        default:
            DEBUG_PRINT("Unexpected node\n");
            exit(1);
    }
    consume(cur);
    return logi;
}

// '(' expr ')'
Node *condtionalExprNode(Token **cur, Var *vars) {
    Node *condi = NULL;
    if ((*cur)->kind == L_PARENTHESES) {
        consume(cur);

        condi = condtionalExprNode(cur, vars);
        
        if ((*cur)->kind != R_PARENTHESES) {
            error("Missing )");
            exit(1);
        }
        consume(cur);

    } else {
        Node *expr = exprNode(cur, vars);
        condi = comparisionOperatorNode(cur);
        condi->left = expr;
        condi->right = exprNode(cur, vars);
    }

    if (isLogicalOperator(*cur)) {
        Node *logi = NULL;  
        logi = logicalOperatorNode(cur, vars);
        logi->left = condi;
        logi->right = condtionalExprNode(cur, vars);
        return logi;
    } else {
        return condi;
    }
}

Node *comparisionOperatorNode(Token **cur) {

    AST_Type type = -1;
    switch ((*cur)->kind) {
        case EQ:
            type = AST_EQ;
            break;
        case NEQ:
            type = AST_NEQ;
            break;
        case LT:
            type = AST_LT;
            break;
        case LTE:
            type = AST_LTE;
            break;
        case GT:
            type = AST_GT;
            break;
        case GTE:
            type = AST_GTE;
            break;
        default:
            DEBUG_PRINT("Unexpected senario %d\n", (*cur)->kind);
            exit(1);
    }

    Node *cmp_op = createNode(type);

    consume(cur);

    return cmp_op;
}

// doesn't consume token
bool isLogicalOperatorInExpr(Token *cur) {
    cur = cur->next; // skip (

    while (cur) {
        TokenKind kind = cur->kind;

        if (kind == EOT) {
            DEBUG_PRINT("missing )\n");
            exit(1);
        }
        
        // to avoid reading excessively
        if (kind == L_BRACE
            || kind == R_BRACE) {
            DEBUG_PRINT("missing )\n");
            exit(1);
        }

        if (kind == R_PARENTHESES) break;

        if (isLogicalOperatorInExpr(cur)) return true;
        cur = cur->next;
    }
    return false;
}

bool isLogicalOperator(Token *tk) {
    switch (tk->kind) {
        case AND:
        case OR:
            return true;
        default:
            return false;
    }
}