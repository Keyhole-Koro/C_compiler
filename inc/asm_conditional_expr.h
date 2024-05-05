#ifndef ASM_CONDI_EXPR_H
#define ASM_CONDI_EXPR_H

#include <stdio.h>

#include "AST.h"

#include "debug.h"

#include "asm_utils.h"
#include "asm_instruction.h"

#include "variable.h"
#include "asm_label.h"
#include "asm_expr.h"

void conditionGen(Node *condi_stmt, Label *trueBranch, Label *falseBranch);


#endif