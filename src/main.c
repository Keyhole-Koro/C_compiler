#include "main.h"



int main() {
    char *input = "int add(int param1, int param2) {\
                        int result = param1 + param2;\
                        return result;\
                    }\
                    int main() {\
                        int a = 1 - 3;\
                        int b = 2;\
                        a = 9 + b + b * 2;\
                        int c = add(a, add(a, b));\
                        return 0;}";

    Token *tokens = tokenize(input);

    showTokens(tokens);

    Node *root = parse(tokens);

    DEBUG_PRINT("%p %p\n", root->left, root->right);

    printPreorder(root, 0, ' ');

    //asmGen(root);
    
    return 0;
}