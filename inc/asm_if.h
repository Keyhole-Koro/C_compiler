#ifndef ASM_IF_H
#define ASM_IF_H

#include <stdio.h>

#include "AST.h"

#include "debug.h"

#include "asm_conditional_expr.h"
#include "asm_label.h"

typedef struct {
    char *labelName;
    Node *if_node;
    Node *next;
} if_node_buffer;

void ifGen(Node *if_node);

#endif