#include "main.h"



int main() {
    char *input = "int add(int param1, int param2) {\
                        int result = param1 + param2;\
                        return result;\
                    }\
                    int main() {\
                        int a = - 9;\
                        int b = - 10;\
                        int c = -11;\
                        int *d = &a;\
                        int e = *d;\
                        int **f = &d;\
                        int g = **f + 3;\
                        int h[3];\
                        h[2] = 4;
                        if ((c >= a) && (a != c)) {\
                           c = add(a, b);\
                        } else if (a >= c) {\
                            c = 1;\
                        } else {\
                            c = 9;\
                        }\
                        return 0;}";

    Token *tokens = tokenize(input);

    //showTokens(tokens);

    Node *root = parse(tokens);

    //printPreorder(root, 0, ' ');

    asmGen(root);
    
    return 0;
}