#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  ND_EXPR,
  ND_OPERATOR,
  ND_NUM,
  ND_END,
} NodeKind;

typedef struct Node Node;

struct Node {
  NodeKind kind;
  Node *ltp;
  Node *next;
  Node *rtp;
  int value;
  char *symbol; // str
};

Node *node;

bool at_end() {
  return node->kind == ND_END;
}

void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

void expect(char op) {
  if (node->kind != ND_OPERATOR || node->symbol[0] != op)
    error("this is not '%c'", op);
  node = node->next;
}

int expect_number() {
  if (node->kind != ND_NUM) {
    error("this is not number"); // replace to error function
  }
  int number = node->value;
  node = node->next;
  return number;
}

bool consume(char op) {
  if (node->kind != ND_OPERATOR || node->symbol[0] != op)
    return false;
  node = node->next;
  return true;
}

Node *new_node(NodeKind kind, Node *current, char *input) {
  Node *tok = calloc(1, sizeof(Node));
  tok->kind = kind;
  tok->symbol = input;
  current->next = tok;
  return tok;
}

Node *nodeize(char *input) {
  Node head;
  head.next = NULL;
  Node *current = &head; // updated everytime

  while (*input) {
    if (isspace(*input)) {
      input++;
      continue;
    }
    if (*input == '+' || *input == '-') {
      current = new_node(ND_OPERATOR, current, input++);
      continue;
    }
    if (isdigit(*input)) {
      current = new_node(ND_NUM, current, input);
      current->value = strtol(input, &input, 10); // this part increments input
      continue;
    }
    error("disable to nodeize");
  }
  new_node(ND_END, current, input);
  return head.next;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("invalid the number of arguments");
    return 1;
  }

  node = nodeize(argv[1]);

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
