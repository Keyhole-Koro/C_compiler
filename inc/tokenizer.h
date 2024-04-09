#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dynamicArray.h"

#include "token.h"
#include "utils.h"

Token *makeToken(int kind, char *val);
DynamicArray *tokenize(char *input);

#endif