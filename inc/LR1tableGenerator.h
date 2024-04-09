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
#include "type.h"
#include "utils.h"

enum {
    NONE = 1,
	TERMIANL_START = 10,
	NUM,      //number
	ADD,      // +
	SUB,      // -
	MUL,      // *
	DIV,      // /
	L_PARENTHESES, // (
	R_PARENTHESES, // )

	PLUS, //+
	MINUS, //-
	ASTARISK, //*
	SLASH, // /

	FIRST,
    END_TERMINAL = 30,
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

//pre defined Productions
//{EXPR, ADD, TERM}
//   0    1     2
Expr preset_expr_instance[] = {
	{0, EXPRRESSION, {EXPRRESSION, END, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{1, EXPRRESSION, {EXPRRESSION, ADD, TERM, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{2, EXPRRESSION, {EXPRRESSION, SUB, TERM, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{3, EXPRRESSION, {TERM, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{4, TERM, {TERM, MUL, FACTOR, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{5, TERM, {TERM, DIV, FACTOR, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{6, TERM, {FACTOR, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{7, FACTOR, {L_PARENTHESES, EXPRRESSION, R_PARENTHESES, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
	{8, FACTOR, {NUM, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE}},
};

Expr *preset_expr = preset_expr_instance;


extern Expr *dummy_expr;
extern Production *dummy_prod;
extern Item *dummy_item;

typedef int state_id;

#endif
