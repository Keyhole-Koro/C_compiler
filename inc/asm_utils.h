#ifndef ASM_UTILS_H
#define ASM_UTILS_H

#include "AST.h"

/**
    x64 ABI conventions
    https://learn.microsoft.com/en-us/cpp/build/x64-software-conventions?view=msvc-170

    registers specifications
    https://i.stack.imgur.com/pWJyr.png

*/

extern char *asm_byte;
extern char *asm_word;
extern char *asm_dword;
extern char *asm_qword;

extern char *asm_di;
extern char *asm_si;
extern char *asm_dx;
extern char *asm_cx;

extern char *asm_r8;
extern char *asm_r9;

char *getWord(int size);

int getVarSize(Node *var);
int getVarOffset(Node *var);

Node *getFuncType(Node *func_details);

char *getParamRegister(int nth, int size);

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
#endif