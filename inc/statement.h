#ifndef STATEMENT_H
#define STATEMENT_H

#include "token.h"
#include "AST.h"

#include "debug.h"

#include "identifier.h"
#include "type.h"

Node *statementNode(Token **cur, int *cur_offset, Var *vars);

#endif