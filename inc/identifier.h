#ifndef IDENTIFIER_H
#define IDENTIFIER_H

#include "token.h"
#include "AST.h"

#include "variable.h"

#include "debug.h"

#include "type.h"
#include "expr.h"

Node *variableNode(Token **cur, Var *vars);

#endif