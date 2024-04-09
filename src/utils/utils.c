#include "utils.h"

inline void error(char ch[]) {
    fprintf(stderr, "%s", ch);
    exit(1);
}

char *readNextUntil(bool (condition)(char), char *buffer, char *ptr_letter) {
  while (!condition(*ptr_letter)) {
      *buffer = *ptr_letter;
      buffer++;
      ptr_letter++;
  }
  *buffer = '\0';
  return ptr_letter;
}

inline bool isEndComment(char *letter) {
  if (*letter == '*' && *(letter + 1) == '/') return true;
  return false;
}

inline bool isIndentation(char *letter) {
  if (*letter == '\\' && *(letter + 1) == 'n') return true;
  return false;
}

inline bool isAlphabet(char *letter) {
	if ((*letter < 'a' || *letter > 'z') && (*letter < 'A' || *letter > 'Z')) return true;
	return false;
}

inline bool isAlphabet_Num(char *letter) {
	if (isAlphabet(letter) && (*letter < '0' || *letter > '9')) return true;
	return false;
}

inline bool isAlphabet_Num_Underbar(char *letter) {
	if (isAlphabet_Num(letter) || *letter == '_') return true;
	return false;
}

inline bool isAlphabet_Underbar(char *letter) {
	if (isAlphabet(letter) && *letter == '_') return true;
	return false;
}