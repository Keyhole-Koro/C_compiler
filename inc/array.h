#ifndef ARRAY_H
#define ARRAY_H

#include "token.h"
#include "AST.h"

#include "variable.h"
#include "type.h"

#include "debug.h"

Node *arrayNode(Token **cur, Var *vars, int *cur_offset);

#endif