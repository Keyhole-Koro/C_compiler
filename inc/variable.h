typedef struct Variable Variable;

struct Variable {
  Variable *next;
  char *name;
  int len;
  int offset; // based on rbp
};

Variable *locals;