#ifndef ASM_UTILS_H
#define ASM_UTILS_H

#include "AST.h"

extern char *asm_word;
extern char *asm_dword;
extern char *asm_qword;

char *getWord(int size);

int getVarSize(Node *var);
int getVarOffset(Node *var);

#endif