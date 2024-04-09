#ifndef UTILITIES_H
#define UTILITIES_H

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))
#define SIZE_OF_A_MEMORY (8)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define IN
#define OUT

void error(char ch[]);

char *readNextUntil(IN bool (*)(char*), OUT char *buffer, IN char *ptr_letter);

bool isEndComment(char *letter);
bool isIndentation(char *letter);
bool isAlphabet_Num_Underbar(char *letter);
bool isAlphabet(char *letter);
bool isAlphabet_Underbar(char *letter);
bool isAlphabet_Num(char *letter);

void updateHashedStateId(int *updatedId, int stateId);

#endif
