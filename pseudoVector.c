#include <stdio.h>
#include <stdlib.h>

typedef struct {
    struct Token *token;
    struct Buffer *buffer_idx;
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

Token *createToken(int tk) {
    Token *token = (Token *)malloc(sizeof(Token));
    if (token == NULL) {
        perror("Failed to allocate memory for token");
        exit(1);
    }
    token->tk = tk;
    return token;
}

Buffer *createBuffer(char *start_idx, char *end_idx) {
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

Vector *createVector_Token_Buffer(int tk, const char *start_idx, const char *end_idx) {
    Vector *vec = createVector();
    vec->token = createToken(tk);
    vec->buffer_idx = createBuffer(start_idx, end_idx);
    return vec;
}

void pushBack(Vector *vec, Vector *addedVec) {
    ensureCapacity(vec, vec->size + 1); // Increase capacity by 1
    vec->token[vec->size] = *(addedVec->token); // Copy token
    vec->buffer_idx[vec->size] = *(addedVec->buffer_idx); // Copy buffer
    vec->size++;
}

int main() {
    Vector *vec = createVector();

    Vector addedVec;
    addedVec.token = createToken(42); // Example token value
    addedVec.buffer_idx = createBuffer("start", "end"); // Example buffer values

    pushBack(vec, &addedVec);

    // Clean up
    free(addedVec.token);
    free(addedVec.buffer_idx);
    free(vec);

    return 0;
}