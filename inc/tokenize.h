#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "utils.h"

Token *tokenize(char *input);

void showTokens(Token *head_tk);
#endif