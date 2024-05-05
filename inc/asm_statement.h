#ifndef ASM_STATEMENT_H
#define ASM_STATEMENT_H

#include <stdio.h>

#include "AST.h"

#include "debug.h"

#include "asm_identifier.h"
#include "asm_if.h"

void stmtGen(Node *root);

#endif