#ifndef UTILITIES_H
#define UTILITIES_H

#define IN
#define OUT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void error(char ch[]);

char *readWhile(bool (*condition)(char*), char *ipt, char **rest);

bool isTab(char *letter);
bool isEnd(char *letter);
bool isEndComment(char *letter);
bool isAlphabet_Num_Underbar(char *letter);
bool isAlphabet(char *letter);
bool isNumber(char *letter);
bool isAlphabet_Underbar(char *letter);
bool isAlphabet_Num(char *letter);

bool isNOTAlphabet_Num_Underbar(char *letter);

bool isInString(char letter, char *str);

#endif
