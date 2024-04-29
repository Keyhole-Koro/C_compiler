#include "main.h"

int main() {
    char *input = "int a = 1 - 3;\
                    int b = 2;\
                    a = 9 + 3;";
    Token *tokens = tokenize(input);

    showTokens(tokens);
    DEBUG_PRINT("a\n");
    Node *root = parse(tokens);
    DEBUG_PRINT("b\n");
    printPreorder(root, 0, ' ');
    DEBUG_PRINT("c\n");

    //asmGen(root);
    
    return 0;
}