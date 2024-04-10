#ifndef VARIABLE_H
#define VARIABLE_H

typedef struct Variable Variable;

struct Variable {
  Variable *next;
  char *name;
  int len;
  int offset; // based on rbp
};

Variable *locals;

#endif