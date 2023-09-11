#include <stdio.h>
#include <stdlib.h>

typedef struct Vector Vector;
typedef struct Token Token;
typedef struct Buffer Buffer;

typedef struct {
    Token *token;
    Buffer *buffer_idx;
    size_t capacity;
    size_t size;
} Vector;

typedef struct {
    int tk;
    // add other fields later
} Token;

typedef struct {
    char *start_idx;
    char *end_idx;
} Buffer;

Vector *createVector() {
    Vector *vec = (Vector *)malloc(sizeof(Vector));
    if (vec == NULL) {
        perror("Failed to allocate memory for vector");
        exit(1);
    }
    vec->token = NULL;
    vec->buffer_idx = NULL;
    vec->size = 0;
    vec->capacity = 0;

    return vec;
}

Token *createToken(int tk) {
    Token *token = (Token *)malloc(sizeof(Token));
    if (token == NULL) {
        perror("Failed to allocate memory for token");
        exit(1);
    }
    token->tk = tk;
    return token;
}

Buffer *createBuffer(const char *start_idx, const char *end_idx) {
    Buffer *buffer = (Buffer *)malloc(sizeof(Buffer));
    if (buffer == NULL) {
        perror("Failed to allocate memory for buffer");
        exit(1);
    }
    buffer->start_idx = start_idx;
    buffer->end_idx = end_idx;
    return buffer;
}

Vector *reallocateVector(Vector *vec, size_t newCapacity) {
    Vector *newVec = (Vector *)realloc(vec, newCapacity * sizeof(Vector));
    if (newVec == NULL) {
        printf("Memory reallocation failed.\n");
        exit(1);
    }
    return newVec;
}

void ensureCapacity(Vector *vec, size_t min_capacity) {
    if (vec->capacity < min_capacity) {
        while (vec->capacity < min_capacity) {
            vec->capacity *= 2;
        }
        Vector *newVec = reallocateVector(vec, vec->capacity);
        // Update the original pointer with the new one
        *vec = *newVec;
        free(newVec);
    }
}

Vector getVec(Vector *vec, int index) {
    if (index < 0 || index >= vec->size) {
        fprintf(stderr, "Error: Index out of bounds\n");
        exit(1);
    }
    return vec[index];
}

Vector *createVector_Token_Buffer(int tk, const char *start_idx, const char *end_idx) {
    Vector *vec = createVector();
    vec->token = createToken(tk);
    vec->buffer_idx = createBuffer(start_idx, end_idx);
    return vec;
}

void pushBack(Vector *vec, Vector *addedVec) {
    ensureCapacity(vec, vec->size + 40);
    vec[vec->size++] = *addedVec;
}
/*
int main() {
    Vector *vec = createVector();

    Vector addedVec;
    // Initialize addedVec here

    pushBack(vec, &addedVec);

    // Clean up
    free(vec);

    return 0;
}
*/
