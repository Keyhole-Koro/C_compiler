#include "main.h"

int main() {
    // Test your tokenize function here
    char input[] = "int main() { return 0; }";
    DynamicArray *tokens = tokenize(input);

    return 0;
}