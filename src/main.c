#include "main.h"

int main() {
    char *input = "5 - 3 / (2 + 1) * 4";
    Token *tokens = tokenize(input);

    //showTokens(tokens);

    Node *root = parse(tokens);

    //printPreorder(root, 0, ' ');

    asmGen(root);
    
    return 0;
}