#include "conditional_expr.h"

Node *cmpOperatorNode(Token **cur);
Node *logicalExprNode(Token **cur, Var *vars);

bool isLogicalOperatorInExpr(Token *cur);
bool isLogicalOperator(Token *tk);

// && ||
Node *logicalExprNode(Token **cur, Var *vars) {
    consume(cur);
    return createNode(((*cur)->kind == AND) ? AST_AND : AST_OR);
}

// '(' expr ')'
Node *condtionalExprNode(Token **cur, Var *vars) {
    if ((*cur)->kind == L_PARENTHESES) {
        consume(cur);

        Node *condi = condtionalExprNode(cur, vars);
        
        if ((*cur)->kind != R_PARENTHESES) {
            error("Expected ')' after expression inside '('");
            exit(1);
        }
        consume(cur);

        return condi;
    } else {
        Node *expr = exprNode(cur, vars);
        Node *condi = cmpOperatorNode(cur);
        condi->left = expr;
        condi->right = exprNode(cur, vars);

        Node *logi = NULL;
        if (isLogicalOperator(*cur)) {
            logi = logicalExprNode(cur, vars);
            logi->left = condi;
            logi->right = condtionalExprNode(cur, vars);
            return logi;
        } else {
            return condi;
        }
    }
}


Node *cmpOperatorNode(Token **cur) {

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