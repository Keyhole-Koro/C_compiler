#include "main.h"

int main() {
    char *input = "1 + 1 * 4";
    Token *tokens = tokenize(input);

    showTokens(tokens);
    
    return 0;
}