#ifndef PRODUCT_H
#define PRODUCT_H

typedef struct Production Production;

struct Production {
    int key;
    Expr *expr;
    int readPosition;
    int cur_symbol;
};

extern Production *dummy_prod;

Production dummy_prod_instance = {-1, &dummy_expr_instance, -1, -1};
Production *dummy_prod = &dummy_prod_instance;

Production *initializeProduction() {
	Production *new_prod = malloc(getDataSize(PRODUCTION));
	*new_prod = *dummy_prod;
	return new_prod;
}

#endif