#include "utilities.h"

int dummy10int[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

typedef struct {
    Type tp;
    size_t sz;
} typeSizeReference;

typeSizeReference dataSizes[] = {
    {INT, sizeof(int)},
    {PRODUCTION, sizeof(Production)},//make a code add keyvalue as function
    {ITEM, sizeof(Item)},
};
/*
typedef struct {
    Type tp;
    Data *dt;
} typeDummyReference;

typeDummyReference dataDummies[] = {
    {INT, (Data *)dummy_int},
    {PRODUCTION, (Data *)dummy_production},//make a code add keyvalue as function
    {ITEM, (Data *)dummy_item},
};
*/
int dummy_int_instance = -1;
int *dummy_int = &dummy_int_instance;

Production dummy_prod_instance = {-1, -1, {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, -1, -1};
Production *dummy_prod = &dummy_prod_instance;

Item dummy_item_instance = {-1, -1, NULL, -1, -1};
Item *dummy_item = &dummy_item_instance;


size_t getDataSize(Type tp) {
    for (int i = 0; i < ARRAY_LENGTH(dataSizes); i++) {
        if (dataSizes[i].tp == tp) return dataSizes[i].sz;
    }
    return 0;
}
/*
Data *getDummy(Type type) {
	for (int i = 0; i < ARRAY_LENGTH(dataDummies); i++) {
        if (dataDummies[i].tp == tp) return dataDummies[i].dt;
    }
    return 0;
}
*/
void error(char ch[]) {
    fprintf(stderr, "%s", ch);
    exit(1);
}

Item *initializeItem() {
	Item *new_item = malloc(getDataSize(ITEM));
	*new_item = *dummy_item;
	return new_item;
}

Production *initializeProduction() {
	Production *new_prod = malloc(getDataSize(PRODUCTION));
	*new_prod = *dummy_prod;
	return new_prod;
}

Item *createItem(int stateId, int readSymbol, Production *prod, int hashed_keys, int hashedStateIds_ItemTransit) {
    Item *new_item = (Item *)malloc(sizeof(Item));
    new_item->stateId = stateId;
    new_item->readSymbol = readSymbol;
    new_item->Productions = prod;
    new_item->hashed_keys = hashed_keys;
	new_item->hashedStateIds_ItemTransit = hashedStateIds_ItemTransit;
    return new_item;
}

void updateHashedStateId(int *updatedId, int stateId) {
	if (*updatedId || stateId == -1) return;
	*updatedId ^= stateId;
}

//update hashkey
//get hashkey
//xor_countdown