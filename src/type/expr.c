#include "expr.h"

Expr dummy_expr_instance = {-1, -1, nullptr};
Expr *dummy_expr = &dummy_expr_instance;

Expr *initializedExpr() {
    Expr *new_expr = malloc(sizeof(Expr));
    *new_expr = *dummy_expr;
    return new_expr;
}