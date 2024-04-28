#include "main.h"

int main() {
    char *input = "(5 - (9 * 2 *1) * ((- 2) * (-4)) + 9) - 0 - (+9)";
    Token *tokens = tokenize(input);

    //showTokens(tokens);

    Node *root = parse(tokens);

    printPreorder(root, 0, ' ');

    asmGen(root);
    
    return 0;
}