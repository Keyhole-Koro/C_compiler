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
