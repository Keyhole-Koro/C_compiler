#ifndef ASM_INSTRUCTION_H
#define ASM_INSTRUCTION_H

#include "AST.h"

#include "debug.h"

extern char *asm_byte;
extern char *asm_word;
extern char *asm_dword;
extern char *asm_qword;

extern char *asm_ax;

extern char *asm_di;
extern char *asm_si;
extern char *asm_dx;
extern char *asm_cx;

extern char *asm_r8;
extern char *asm_r9;

extern char *asm_je;
extern char *asm_jne;
extern char *asm_jnae;
extern char *asm_ja;
extern char *asm_jna;
extern char *asm_jae;
extern char *asm_jnge;
extern char *asm_jg;
extern char *asm_jge;
extern char *asm_jng;

char *getWord(int size);
char *getParamRegister(int nth, int size);
char *getAXRegister(int size);

char *getJumpInstruct_unsigned(Node *node);
char *getJumpInstruct_signed(Node *node);

#endif