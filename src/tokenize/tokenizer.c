#include "tokenizer.h"

Token *createToken(Token *cur, int kind, char *value);

Token *tokenize(char *input) {
  Token *cur = calloc(1, sizeof(Token));
  cur->next = NULL;
  Token *head = cur;

  char *ptr = input;
  char *rest = NULL;

  while (*ptr) {
    
    if (isspace(*ptr) || isTab(ptr)) {
      ptr++;
      continue;
    }

    if (*ptr == '/' && *(ptr + 1) == '/') {
      char *buf = readWhile(isEnd, ptr, &rest);
      free(buf);
      ptr = rest;
      continue;
    }

    if (*ptr == '*' && *(ptr + 1) == '/') {
      char *buf = readWhile(isEndComment, ptr, &rest);
      free(buf);
      ptr = rest;
      continue;
    }
    
    if (strchr("+-*/;=(){},<>[]&.!?:|^%~#", *ptr)) {
      char char_buf[2] = {*ptr, '\0'};
      TokenKind kind = findTokenKind(&char_buf);
      cur = createToken(cur, kind, NULL);
      ptr++;
      continue;
    }

    if (isNumber(ptr)) {
      char *buf = readWhile(isNumber, ptr, &rest);
      cur = createToken(cur, NUMBER, buf);
      ptr = rest;
      continue;
    }

    if (isAlphabet_Underbar(ptr)) {
      char *str = readWhile(isAlphabet_Num_Underbar, ptr, &rest);
      TokenKind kind = findTokenKind(str);
      // -1 indicates token kind not found
      if (kind == (TokenKind)-1) kind = IDENTIFIER;
      cur = createToken(cur, kind, str);
      ptr = rest;
      continue;
    }
    
    if (!isAlphabet_Num_Underbar(ptr)) {
      char *str = readWhile(isNOTAlphabet_Num_Underbar, ptr, &rest);
      TokenKind kind = findTokenKind(str);
      cur = createToken(cur, IDENTIFIER, str);
      ptr = rest;
      continue;
      
    }
    error("token kind not found");
    ptr++;
  }

  return head->next;
}

Token *createToken(Token *cur, int kind, char *value) {
  Token *newTk = malloc(sizeof(Token));
  newTk->kind = kind;
  newTk->value = value;
  newTk->next = NULL;
  cur->next = newTk;
  return newTk;
}
