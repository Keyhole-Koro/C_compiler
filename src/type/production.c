#include "production.h"

Production dummy_prod_instance = {-1, -1, NULL, -1};
Production *dummy_prod = &dummy_prod_instance;

Production *initializedProduction() {
	Production *new_prod = malloc(sizeof(Production));
	*new_prod = *dummy_prod;
	return new_prod;
}

inline void updateCur_symbol(Production *prod) {
    prod->cur_symbol = getRightFromProd((void*)prod, prod->readPosition, retriveDataType("prod"));
}

inline int lookAheadSymbol(Production *prod) {
    return prod->expr->right[prod->readPosition + 1];
}

inline int getKeyFromProd(void *data, DataType *dataType) {
	if (strcmp(dataType->name, "prod")) error("type mismatch: getKeyFromProd\n");
	return ((Production*)data)->key;
}

inline int getLeftFromProd(void *data, DataType *dataType) {
	if (strcmp(dataType->name, "prod")) error("type mismatch: getLeftFromProd\n");
	return ((Production*)data)->expr->left;
}

inline int getRightFromProd(void *data, int index, DataType *dataType) {
    if (strcmp(dataType->name, "prod")) error("type mismatch: getLeftFromProd\n");
    return ((Production*)data)->expr->right[index];
}


inline int getCur_symbol(void *data, DataType *dataType) {
	if (strcmp(dataType->name, "prod")) error("type mismatch: getCur_symbol\n");
	Production *prod = (Production *)(data);
	return prod->cur_symbol;
}

inline int getCur_symbolForOverlapHandler(void *data, DataType *dataType) {
	if (strcmp(dataType->name, "prod" && datastrcmp(dataType->name, "int"))) error("type mismatch: getCur_symbolForOverlapHandler\n");
	Production *prod = (Production *)(data);
	return prod->cur_symbol - NON_TERMINAL_START;
}

inline bool cmpCur_symbol(void *data, void *expectedData, DataType *dataType) {
	if (strcmp(dataType->name, "prod")) error("type mismatch: cmpCur_symbol\n");
	Production *symbol = (Production *)data;
	Production *compedValue = (Production *)expectedData;
	return symbol->cur_symbol == compedValue->cur_symbol;
}

inline bool cmpLeftFromProd(void *data, void *expectedValue, DataType *dataType) {
	if (strcmp(dataType->name, "prod")) error("type mismatch: cmpLefFromProd\n");
	return getLeftFromProd(data, dataType) == getLeftFromProd(expectedValue, dataType);
}