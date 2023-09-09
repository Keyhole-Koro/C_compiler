#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))
#define SIZE_OF_A_MEMORY (8)

#ifndef UTILITIES_H
#define UTILITIES_H
typedef enum {
    INT = 128,
    PRODUCTION,
    ITEM,
} Type;

struct typeSizeReference {
    Type tp;
    size_t dataSize;
};

struct typeSize sizes[] = {
    {INT, sizeof(int)},
    {PRODUCTION, sizeof(Production)},
    {ITEM, sizeof(Item)},
};

size_t getDataSize(Type tp) {
    for (int i = 0, i < ARRAY_LENGTH(sizes), i++) {
        if (sizes[i].tp == tp) return sizes[i].dataSize;
    }
    return 0;
}
#endif
