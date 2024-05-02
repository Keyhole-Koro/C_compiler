#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include "token.h"
#include "AST.h"

#include "debug.h"

#include "type.h"
#include "expr.h"

Node *ifNode(Token **cur, int *cur_offset, Var *vars);

#endif