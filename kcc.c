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

int expect_number() {
  if (token->kind != TK_NUM) {
    printf("this is not number"); // replace to error function
  }
  int number = token->value;
  token = token->next;
  return number;
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
    // error("disable to tokenize");
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

  while (!at_end()) {
    if (token->kind == TK_OPERATOR) {
      token = token->next;
      if (strcmp(token->symbol, "+") == 0) {
        printf("  add rax, %d\n", expect_number());
      } else if (strcmp(token->symbol, "-") == 0) {
        printf("  sub rax, %d\n", expect_number());
      }
    }
    token = token->next;
  }

  printf("  ret\n");
  return 0;
}
