#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    ADD,      // +
    SUB,      // -
    MUL,      // *
    DIV,      // /
    MOD,      // %
    ASSIGN,   // =
    EQ,       // ==
    NEQ,      // !=
    LT,       // <
    GT,       // >
    LTE,      // <=
    GTE,      // >=
    AND,      // &&
    OR,       // ||
    NOT,      // !
    BITAND,   // &
    BITOR,    // |
    BITXOR,   // ^
    BITNOT,   // ~
    SHL,      // <<
    SHR,      // >>
    INC,      // ++
    DEC,      // --

    POINTER,  // (*)
    ADDRESS,  // &
    MEMBER,   // ->
    SIZEOF,   // sizeof

    BOOL,     // bool
    INT,      // int data type
    CHAR,     // char data type
    FLOAT,    // float data type
    DOUBLE,   // double data type
    VOID,     // void data type
    LONG,     // long data type
    SHORT,    // short data type
    UNSIGNED, // unsigned modifier
    SIGNED,   // signed modifier
    CONST,    // const modifier
    STATIC,   // static modifier
    EXTERN,   // extern modifier
    AUTO,     // auto modifier
    REGISTER, // register modifier

    IF,       // if
    ELSE,     // else
    WHILE,    // while
    DO,       // do
    FOR,      // for
    SWITCH,   // switch
    CASE,     // case
    DEFAULT,  // default
    BREAK,    // break
    CONTINUE, // continue
    RETURN,   // return
    TYPEDEF,  // typedef
    STRUCT,   // struct
    UNION,    // union
    ENUM,     // enum
    SIZEOF,   // sizeof

    VARIABLE, //defined by user
} Keyword;



typedef struct Token Token;

struct Token {
  Keyword kw;
  int value;
  char *str;
  struct Token *next;
};

Token tokenize(char *input){
    Token *head = calloc(1, sizeof(Token));
    head->kw = -1;
    head->value = '\n';
    head->next = NULL;
    Token *cur_token = head;
    char *ipt = input;

    Keyword *idfd_kw;//identified keyword

    while(*ipt){
      if (isspace(*ipt)) {
        ipt++;
        continue;
      } else {
        idfd_kw = kw_idfr(ipt++);
      }
    }
}

Token *makeToken(Keyword kind, Token *cur_token, char *ipt){
  Token *new_token = calloc(1, sizeof(Token));
  new_token->kw = kind;
  new_token->value = *ipt;
  cur_token->next = new_token;

  return new_token;
}

Keyword *kwIdfr(char *ipt){//keyword identifier   
    switch ()
    {
    case /* constant-expression */:
        /* code */
        break;
    
    default:
        break;
    }
}