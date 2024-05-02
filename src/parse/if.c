#include "if.h"

// defined in statement.c
Node *statementNode(Token **cur, int *cur_offset, Var *vars);

Node *condtionalExprNode(Token **cur, Var *vars);
Node *cmpOperatorNode(Token **cur);
bool isLogicalOperatorInExpr(Token *cur);
bool isLogicalOperator(Token *tk);

Node *ifNode(Token **cur, int *cur_offset, Var *vars) {

    Node *if_node = NULL;
    switch ((*cur)->kind) {
        case IF:
            consume(cur);
            if_node = createNode(AST_IF);
            break;
        case ELSE:
            consume(cur);
            if ((*cur)->kind == IF) {
                if_node = createNode(AST_ELSE_IF);
                consume(cur);
                break;
            }
            if_node = createNode(AST_ELSE);
            break;
        default:
            DEBUG_PRINT("out of senario\n");
            exit(1);
    }

    Node *condi_stmt = createNode(AST_CONDITIONAL_STATEMTNT);

    if_node->left = condi_stmt;

    if (if_node->type != AST_ELSE) {
        // '(' expr ')'

        expect(*cur, L_PARENTHESES);
        consume(cur);
        
        condi_stmt->left = condtionalExprNode(cur, vars);

        expect(*cur, R_PARENTHESES);
        consume(cur);
    }
    DEBUG_PRINT("%s\n", revertToken((*cur)->next));
    // support a line stmt later
    expect(*cur, L_BRACE);
    consume(cur);

    condi_stmt->right = statementNode(cur, cur_offset, vars);

    expect(*cur, R_BRACE);
    consume(cur);

    if ((*cur)->kind == ELSE && if_node->type != AST_ELSE) if_node->right = ifNode(cur, cur_offset, vars); 

    return if_node;
}

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