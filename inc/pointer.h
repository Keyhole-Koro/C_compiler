#ifndef POINTER_H
#define POINTER_H

#include "token.h"
#include "AST.h"

#include "debug.h"

#include "asm_utils.h"

Node *pointerOperatorNode(Token **cur);
Node *getPointerNode(Node *var);
int getNumPointer(Node *pointer_node);
int getNumDerefenceOperator(Node *derefence);

#endif