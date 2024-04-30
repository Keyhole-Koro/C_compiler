#ifndef ASM_STATEMENT_H
#define ASM_STATEMENT_H

#include <stdio.h>

#include "AST.h"

#include "debug.h"

#include "asm_identifier.h"

void stmtGen(Node *root);

#endif