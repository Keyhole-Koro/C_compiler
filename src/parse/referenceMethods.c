#include "referenceMethods.h"

bool isNonTerminal(int symbol) {
	return symbol > NON_TERMINAL_START;
}

int getIntFromData(void *data, DataType *type) {
	if (type->name != "int") error("type mismatch: getIntFromData\n");
	return *(int *)data;
}

int getIntFromDataForSymbol(void *data, DataType *type) {
	if (type->name != "int") error("type mismatch: getIntFromDataForSymbol\n");
    int sym = *(int *)data;
    if (sym > NON_TERMINAL_START) return abs(sym - NON_TERMINAL_START) + END_TERMINAL;
    if (TERMIANL_START < sym && sym < END_TERMINAL) return sym;
    return NONE;
}

int getKeyFromProd(void *data, DataType *type) {
	if (type->name != "production") error("type mismatch: getKeyFromProd\n");
	return ((Production*)data)->key;
}

int getLeftFromProd(void *data, DataType *type) {
	if (type->name != "production") error("type mismatch: getLeftFromProd\n");
	return ((Production*)data)->expr->left;
}

int getRightFromProd(void *data, int index, DataType *type) {
    if (type->name != "production") error("type mismatch: getLeftFromProd\n");
    return ((Production*)data)->expr->right[index];
}


int getCur_symbol(void *data, DataType *type) {
	if (type->name != "production") error("type mismatch: getCur_symbol\n");
	Production *prod = (Production *)(data);
	return prod->cur_symbol;
}

int getCur_symbolForOverlapHandler(void *data, DataType *type) {
	if (type->name != "production" && type->name != "int") error("type mismatch: getCur_symbolForOverlapHandler\n");
	Production *prod = (Production *)(data);
	return prod->cur_symbol - NON_TERMINAL_START;
}

bool cmpCur_symbol(void *data, Data *expectedData, DataType *type) {
	if (type->name != "production") error("type mismatch: cmpCur_symbol\n");
	Production *symbol = (Production *)data;
	Production *compedValue = (Production *)expectedData;
	return symbol->cur_symbol == compedValue->cur_symbol;
}

bool cmpHash_keyOfProdsInItem(void *data, Data *expectedData, DataType *type) {
	if (type->name != ITEM) error("type mismatch: cmpHash_keyOfProdsInItem\n");
	Item *item = (Item *)data;
	Item *cmpedItem = (Item *)expectedData;
	return item->hashed_keys == cmpedItem->hashed_keys;
}

bool cmp_readSymbolHash_keyOfProdsInItem(void *data, Data *expectedData, DataType *type) {
	if (type->name != ITEM) error("type mismatch: cmpHash_keyOfProdsInItem\n");
	Item *item = (Item *)data;
	Item *cmpedItem = (Item *)expectedData;
	if (item->hashed_keys != cmpedItem->hashed_keys) return false;
	if (item->readSymbol != cmpedItem->readSymbol) return false;
	return true;
}

int getHash_keyOfProdsInItem(void *data, DataType *type) {
	if (type->name != ITEM) error("type mismatch: getHash_keyOfProdsInItem\n");
	Item *item = (Item *)data;
	return item->hashed_keys;
}

bool cmpLeftFromProd(void *data, Data *expectedValue, DataType *type) {
	if (type->name != "production") error("type mismatch: cmpLefFromProd\n");
	return getLeftFromProd(data, PRODUCTION) == getLeftFromProd(expectedValue, PRODUCTION);
}

bool cmpStateId(void *data, Data *expectedValue, DataType *type) {
	if (type->name != ITEM) error("type mismatch: cmpStateId\n");
	Item *item = (Item *)data;
	int *compedValue = (int*)expectedValue;
	return item->stateId == *compedValue;
}

bool cmpReadSymbol(Data* data, Data* expectedValue, DataType *type) {
	if (type->name != ITEM) error("type mismatch: cmpReadSymbol\n");
	Item *item = (Item *)data;
	int *compedValue = (int*)expectedValue;
	return item->readSymbol == *compedValue;
}

void updateCur_symbol(Production *prod) {
    prod->cur_symbol = getRightFromProd((Data*)prod, prod->readPosition, PRODUCTION);
}

int lookAheadSymbol(Production *prod) {
    return prod->expr->right[prod->readPosition + 1];
}
