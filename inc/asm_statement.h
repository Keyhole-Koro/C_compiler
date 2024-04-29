#ifndef ASM_GEN_H
#define ASM_GEN_H

#include <stdio.h>

#include "AST.h"

#include "debug.h"

#include "asm_statement.h"
#include "asm_expr.h"

void stmtGen(Node *root);

#endif