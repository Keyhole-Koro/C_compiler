#include "utilities.h"

typedef struct {
    Type tp;
    size_t sz;
} typeSizeReference;

typeSizeReference dataSizes[] = {
    {INT, sizeof(int)},
    {PRODUCTION, sizeof(Production)},//make a code add keyvalue as function
    {ITEM, sizeof(Item)},
};

size_t getDataSize(Type tp) {
    for (int i = 0; i < ARRAY_LENGTH(dataSizes); i++) {
        if (dataSizes[i].tp == tp) return dataSizes[i].sz;
    }
    return 0;
}


void error(char ch[]) {
    fprintf(stderr, "%s", ch);
    exit(1);
}

void initializeItem(Item *item) {
	item->stateId = -1;
	item->transitionedSymbol = -1;
	item->Productions = dummy_prod;
	item->hashed_keys = -1;
}

void initializeProduction(Production *prod) {
	prod->key = -1;
	prod->left = -1;
	prod->right = (int *){-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	prod->readPosition = -;
	prod->cur_symbol = -1;
}

Item *setItem(int stateId, int transitionedSymbol, int hashed_keys) {
    Item *new_item = (Item *)malloc(sizeof(Item));
    new_item->stateId = stateId;
    new_item->transitionedSymbol = transitionedSymbol;
    new_item->Productions = (Production *)fetchedProdArray;
    new_item->hashed_keys = hashed_keys;
    return new_item;
}
