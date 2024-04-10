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

#include "LR1_token.h"

extern Expr *dummy_expr;
extern Production *dummy_prod;
extern Item *dummy_item;

typedef int state_id;

#endif
