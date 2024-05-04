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

/*
RDI: 1
RSI: 2
RDX: 3
RCX: 4
R8: 5
R9: 6

8bytes
"mov rax, qword [rbp - ]"
4bytes
"mov eax, dword [rbp - ]"



8bytes
"mov rdi, qword [rbp - ]"

4bytes
"
mov rax, qword [rbp - ]
mov edi, eax"

2bytes
"
mov rax, qword [rbp - ]
mov di, ax"

1byte
"
mov rax, qword [rbp - ]
mov dil, al"

when receiving arguments

8bytes
"mov qword [rbp - ], rdi"

4bytes
"mov "
*/

/*
AST_EQ je
AST_NEQ jne

a above
b below

g greater
l less
        unsigned    signed
AST_LT  jnae jb     jnge jl
AST_GT  ja   jnbe   jg   jnle
AST_LTE jna jnb     jge  jnl
AST_GTE jae jbe     jng  jle
*/

#endif