#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"

bool isEndComment(char *letter);
bool isIndentation(char *letter);
bool isAlphabet_Num_Underbar(char *letter);
bool isAlphabet(char *letter);
bool isAlphabet_Underbar(char *letter);
bool isAlphabet_Num(char *letter);

Token *makeToken(int kind, char *ipt);
DynamicArray *tokenize(char *input);

char *readNextUntil(bool (*condition)(char), char *ptr_letter);

bool ifMatch(char *target, char *pattern, size_t read_size);
int identifier(char *ipt, char *end_target, struct KeyValue *symbols, bool (*condition)(char));
int getValue(char *substring, size_t substring_length, struct KeyValue *symbols);

void initialize()

#endif