#include "main.h"

int main() {
    char *input = "int main() { return 0; }";
    Token *tokens = tokenize(input);

    showTokens(tokens);
    
    return 0;
}