#ifndef ASM_IDENTIFIER_H
#define ASM_IDENTIFIER_H

#include "AST.h"

#include "asm_utils.h"
#include "debug.h"

#include "asm_expr.h"

void assignGen(Node *assign);
void declareGen(Node *declare);

#endif