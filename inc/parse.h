#ifndef PARSE_H
#define PARSE_H

#include "AST.h"
#include "token.h"

#include "expr.h"

Node *parse(Token *hive_token);

#endif