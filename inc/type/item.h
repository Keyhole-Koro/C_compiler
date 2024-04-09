#ifndef ITEM_H
#define ITEM_H

typedef struct Item Item;

struct Item {
    int stateId;
    int readSymbol;
    int arraySize;
    Production **Productions;
    int hashed_keys;//of keys of productions
};

extern Item *dummy_item;

Item dummy_item_instance = {-1, -1, -1, &dummy_prod, -1};
Item *dummy_item = &dummy_item_instance;

Item *initializeItem() {
	Item *new_item = malloc(getDataSize(ITEM));
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


#endif