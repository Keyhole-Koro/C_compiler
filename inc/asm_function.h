#ifndef ASM_FUNCTION_H
#define ASM_FUNCTION_H

#include <stdio.h>

#include "AST.h"

#include "debug.h"
#include "asm_utils.h"
#include "asm_instruction.h"

#include "asm_expr.h"
#include "asm_statement.h"

void funcGen(Node *root);

void callFuncGen(Node *callFunc);

int getReturnSize(Node *func);

#endif