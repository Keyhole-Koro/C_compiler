#include "main.h"

int main() {
    char *input = "int a = 1 - 3;\
                    int b = 2;\
                    a = 9 + 3;";
    Token *tokens = tokenize(input);

    showTokens(tokens);

    Node *root = parse(tokens);

    printPreorder(root, 0, ' ');

    asmGen(root);
    
    return 0;
}