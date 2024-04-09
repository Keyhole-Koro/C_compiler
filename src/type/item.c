#include "item.h"

Item dummy_item_instance = {-1, -1, -1, -1, &dummy_prod};
Item *dummy_item = &dummy_item_instance;

Item *initializedItem() {
	Item *new_item = malloc(sizeof(Item));
	*new_item = *dummy_item;
	return new_item;
}

Item *createItem(int stateId, int readSymbol, int arraySize, Production **prod, int hashed_keys) {
    Item *new_item = (Item *)malloc(sizeof(Item));
    new_item->stateId = stateId;
    new_item->readSymbol = readSymbol;
    new_item->arraySize = arraySize;
    new_item->Productions = prod;
    new_item->hashed_keys = hashed_keys;
    return new_item;
}

void updateHashedStateId(int *updatedId, int stateId) {
	if (*updatedId || stateId == -1) return;
	*updatedId ^= stateId;
}

bool cmpStateId(void *data, void *expectedData, DataType *dataType) {
	if (strcmp(dataType->name, "item")) error("type mismatch: cmpStateId\n");
	Item *item = (Item *)data;
	int *compedValue = (int*)expectedData;
	return item->stateId == *compedValue;
}

bool cmpReadSymbol(void *data, void *expectedValue, DataType *dataType) {
	if (strcmp(dataType->name, "item")) error("type mismatch: cmpReadSymbol\n");
	Item *item = (Item *)data;
	int *compedValue = (int*)expectedValue;
	return item->readSymbol == *compedValue;
}

bool cmpHash_keyOfProdsInItem(void *data, void *expectedData, DataType *dataType) {
	if (strcmp(dataType->name, "item")) error("type mismatch: cmpHash_keyOfProdsInItem\n");
	Item *item = (Item *)data;
	Item *cmpedItem = (Item *)expectedData;
	return item->hashed_keys == cmpedItem->hashed_keys;
}

bool cmp_readSymbolHash_keyOfProdsInItem(void *data, void *expectedData, DataType *dataType) {
	if (strcmp(dataType->name, "item")) error("type mismatch: cmpHash_keyOfProdsInItem\n");
	Item *item = (Item *)data;
	Item *cmpedItem = (Item *)expectedData;
	if (item->hashed_keys != cmpedItem->hashed_keys) return false;
	if (item->readSymbol != cmpedItem->readSymbol) return false;
	return true;
}

int getHash_keyOfProdsInItem(void *data, DataType *dataType) {
	if (strcmp(dataType->name, "item")) error("type mismatch: getHash_keyOfProdsInItem\n");
	Item *item = (Item *)data;
	return item->hashed_keys;
}

