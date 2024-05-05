#ifndef ASM_UTILS_H
#define ASM_UTILS_H

#include "AST.h"

/**
    x64 ABI conventions
    https://learn.microsoft.com/en-us/cpp/build/x64-software-conventions?view=msvc-170

    registers specifications
    https://i.stack.imgur.com/pWJyr.png

*/


int getVarSize(Node *var);
int getVarOffset(Node *var);

Node *getFuncType(Node *func_details);

#define expectNode(node, expectedType) \
    do { \
        if ((node)->type != (expectedType)) { \
            DEBUG_PRINT("Unexpected Node\n"); \
            exit(1); \
        } \
    } while(0)

#endif