#include "main.h"



int main() {
    char *input = "int add(int param1, int param2) {\
                        int result = param1 + param2;\
                        return result;\
                    }\
                    int main() {\
                        int a = - 9;\
                        int b = 2;\
                        int c = 3;\
                        if (a > b || (b <= a && a == b)) {\
                           c = add(a, b);\
                        } else {\
                            c = 1;\
                        }\
                        return 0;}";

    Token *tokens = tokenize(input);

    //showTokens(tokens);

    Node *root = parse(tokens);

    printPreorder(root, 0, ' ');

    //asmGen(root);
    
    return 0;
}