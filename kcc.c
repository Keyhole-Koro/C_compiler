#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  tk_num,
  tk_charactor,
  tk_eof
} tokenkind;

static struct Token {
  tokenkind kind;
  int value;
  char symbol;
  struct Token *next;
};

typedef enum {
  nd_unaryPlus,
  nd_numMinus,
  nd_plus,
  nd_minus,
  nd_mul,
  nd_div,
  nd_lpths,
  nd_rpths
} nodesymbol;

static struct Node {
  nodesymbol symbol;
  int value;//only contains number
  struct Node *lnode;
  struct Node *rnode;
};

Token *add_token(tokenkind kind, Token *cur_token, char *ipt) {
  Token *new_token = calloc(1, sizeof(Token));
  new_token->kind = kind;
  new_token->value = *ipt;
  new_token->next = NULL;

  if (cur_token) {
      cur_token->next = new_token;
  }

  return new_token;
}

Token *tokenize(char *input) {
  Token *head = malloc(1, sizeof(Token));
  head->kind = -1;
  head->value = '\n';
  head->next = NULL;
  Token *cur_token = head;
  char *ipt = input;

  while (*ipt) {
      if (isspace(*ipt)) {
          ipt++;
          continue;
      }
      if (isdigit(*ipt)) {
          cur_token = add_token(tk_num, cur_token, ipt++);
          continue;
      }
      if (*ipt == '+' || *ipt == '-' || *ipt == '*' || *ipt == '/' || *ipt == '(' || *ipt == ')') {
          cur_token = add_token(tk_charactor, cur_token, ipt++);
          continue;
      }
      // If none of the above conditions matched, it means the input contains an invalid character.
      printf("Invalid character: %c\n", *ipt);
      //error("invalid syntax");//have yet to defiine
  }
  return head;
}


int main(int argc, char **argv) {
  if (argc != 2) {
    printf("invalid the number of arguments");
    return 1;
  }
  Token token = tokenize(argv[1]);//also used for error detection later
  /*
  Node node = expr(token);

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
  */
  return 0;
}
//add error exit(1)