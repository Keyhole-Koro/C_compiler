#include "utilities.h"

size_t getDataSize(Type tp) {
    for (int i = 0; i < ARRAY_LENGTH(sizes); i++) {
        if (sizes[i].tp == tp) return sizes[i].dataSize;
    }
    return 0;
}
