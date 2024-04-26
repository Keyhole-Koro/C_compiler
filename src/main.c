#include "main.h"

int main() {
    char *input = "int main() { return 0; }";
    Token *tokens = tokenize(input);

    for (Token *tk = tokens; tk; tk = tk->next) {
        printf("kind %d value %s\n", tk->kind, tk->value);
    }

    return 0;
}