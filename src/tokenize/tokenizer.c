#include "tokenizer.h"

DynamicArray *tokenize(char *input) {
  DataType *Tk = registerDataType("token", sizeof(TokenKind));

  DynamicArray *arr = createDynamicArray("Tokens", 200, true, true, NULL, Tk);

  char *ipt = input;
  char *idfd_val = NULL;

  while (*ipt) {
    if (isspace(*ipt)) {
      ipt++;
      continue;
    }

    if (isIndentation(ipt)) {
      ipt += 2;
      continue;
    }

    if (*ipt == '/' && *(ipt + 1) == '/') { // //
      char *dummy = NULL;
      char *end_target = readNextUntil(isIndentation, dummy, ipt);
      ipt = end_target;
      continue;
    }

    if (*ipt == '/' && *(ipt + 1) == '*') { // /*
      char *dummy = NULL;
      char *end_target = readNextUntil(isEndComment, dummy, ipt);
      ipt = end_target;
      continue;
    }

    if (strchr("+-*/;=(){},<>[]&.!?:|^%~#", *ipt)) {
      TokenKind kind = findCorrespondToken(ipt++, 1, single_char);
      addToDynamicArray(arr, makeToken(kind, NULL), Tk);
      continue;
    }

    if (isAlphabet_Underbar(ipt)) {
      char *idfr_buffer = NULL;
      char *end_target = readNextUntil(isAlphabet_Num_Underbar, idfr_buffer, ipt);
      addToDynamicArray(arr, makeToken(IDENTIFIER, idfd_val), Tk);
      ipt = end_target;
      continue;
    }
    
    if (!isAlphabet_Num_Underbar(ipt)) {
      char *idfr_buffer = NULL;
      char *end_target = readNextUntil(!isAlphabet_Num_Underbar, idfr_buffer, ipt);
      TokenKind kind = findCorrespondToken(ipt++, 1, single_char);
      // if (kind == -1) failed to tokenize
      ipt = end_target;
      continue;
    }
  }

  return arr;
}
