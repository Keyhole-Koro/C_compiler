#ifndef EXPR_H
#define EXPR_H

typedef struct Expr Expr;

struct Expr {
    int key;
    int left;
    int right[10];
};

Expr dummy_expr_instance = {-1, -1, {-1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
Expr *dummy_expr = &dummy_expr_instance;

Item *initializeExpr() {
    Item *new_expr = malloc(getDataSize(EXPR));
    *new_expr = *dummy_item;
    return new_expr;
}


#endif