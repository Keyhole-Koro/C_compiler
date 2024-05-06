#ifndef ASM_GEN_H
#define ASM_GEN_H

#include <stdio.h>

#include "AST.h"

#include "asm_instruction.h"

#include "asm_utils.h"
#include "debug.h"

#include "pointer.h"

void exprGen(Node *root);

#endif