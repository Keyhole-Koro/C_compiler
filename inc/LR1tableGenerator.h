#ifndef TABLEGENERATOR_H
#define TABLEGENERATOR_H

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dynamicArray.h"

#include "token.h"
#include "type.h"
#include "utils.h"

enum {
    NONE = =1,

	NON_TERMINAL_START = 256,
	EXPRRESSION, // E
	TERM, // T
	FACTOR, // F
	ACCEPTED, //accept

	END, //$ temporary

	S,  // shift
	R, // reducce
	G, //goto
	ACC, //acc
};

extern Expr *dummy_expr;
extern Production *dummy_prod;
extern Item *dummy_item;

typedef int state_id;

void registerSyntax(Token left, Token *right);

#endif
