#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  TK_OPERATOR,
  TK_NUM,
  TK_END,
} TokenKind;

typedef struct Token Token;

struct Token {
  TokenKind kind;
  Token *next;
  int value;
  char *symbol; // str
};

Token *token;

bool at_end() {
  return token->kind == TK_END;
}

void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

void expect(char op) {
  if (token->kind != TK_OPERATOR || token->symbol[0] != op)
    error("this is not '%c'", op);
  token = token->next;
}

int expect_number() {
  if (token->kind != TK_NUM) {
    error("this is not number"); // replace to error function
  }
  int number = token->value;
  token = token->next;
  return number;
}

bool consume(char op) {
  if (token->kind != TK_OPERATOR || token->symbol[0] != op)
    return false;
  token = token->next;
  return true;
}

Token *new_token(TokenKind kind, Token *current, char *input) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->symbol = input;
  current->next = tok;
  return tok;
}

Token *tokenize(char *input) {
  Token head;
  head.next = NULL;
  Token *current = &head; // updated everytime

  while (*input) {
    if (isspace(*input)) {
      input++;
      continue;
    }
    if (*input == '+' || *input == '-') {
      current = new_token(TK_OPERATOR, current, input++);
      continue;
    }
    if (isdigit(*input)) {
      current = new_token(TK_NUM, current, input);
      current->value = strtol(input, &input, 10); // this part increments input
      continue;
    }
    error("disable to tokenize");
  }
  new_token(TK_END, current, input);
  return head.next;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("invalid the number of arguments");
    return 1;
  }

  token = tokenize(argv[1]);

  printf(".intel_syntax noprefix\n");
  printf(".extern printf\n");
  printf(".globl main\n");
  printf("main:\n");

  printf("  mov rax, %d\n", expect_number());
  //expect_number proceeds next
  while (!at_end()) {
    if (consume('+')) {//doesnt detect error
      printf("  add rax, %d\n", expect_number());
      continue;
    }
    expect('-');//detect error
    printf("  sub rax, %d\n", expect_number());
  }

  printf("  ret\n");
  return 0;
}
