#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
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
    INT,      // int
    CHAR,     // char
    FLOAT,    // float
    DOUBLE,   // double
    VOID,     // void
    LONG,     // long
    SHORT,    // short
    UNSIGNED, // unsigned
    SIGNED,   // signed
    CONST,    // const
    STATIC,   // static
    EXTERN,   // extern
    AUTO,     // auto
    REGISTER, // register

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
};



typedef struct Token Token;

struct Token {
  int kw;
  int value;
  char *str;
  struct Token *next;
};

struct {
  char *operator;
  int op;
} symbols[] = {
  {"+", ADD},
  {"-", SUB},
  {"*", MUL},
  {"/", DIV},
  {"%", MOD},
  {"=", ASSIGN},
  {"==", EQ},
  {"!=", NEQ},
  {"<", LT},
  {">", GT},
  {"<=", LTE},
  {">=", GTE},
  {"&&", AND},
  {"||", OR},
  {"!", NOT},
  {"&", BITAND},
  {"|", BITOR},
  {"^", BITXOR},
  {"~", BITNOT},
  {"<<", SHL},
  {">>", SHR},
  {"++", INC},
  {"--", DEC},
  {"*", POINTER},
  {"&", ADDRESS},
  {"->", MEMBER},
};//optimize later

struct {
  char *keyword;
  int kw;
} symbols[] = {
  {"sizeof", SIZEOF},
  {"bool", BOOL},
  {"int", INT},
  {"char", CHAR},
  {"float", FLOAT},
  {"double", DOUBLE},
  {"void", VOID},
  {"long", LONG},
  {"short", SHORT},
  {"unsigned", UNSIGNED},
  {"signed", SIGNED},
  {"const", CONST},
  {"static", STATIC},
  {"extern", EXTERN},
  {"auto", AUTO},
  {"register", REGISTER},
  {"if", IF},
  {"else", ELSE},
  {"while", WHILE},
  {"do", DO},
  {"for", FOR},
  {"switch", SWITCH},
  {"case", CASE},
  {"default", DEFAULT},
  {"break", BREAK},
  {"continue", CONTINUE},
  {"return", RETURN},
  {"typedef", TYPEDEF},
  {"struct", STRUCT},
  {"union", UNION},
  {"enum", ENUM},
  {"sizeof", SIZEOF},
  {"variable", VARIABLE},
}

Token tokenize(char *input){
    Token *head = calloc(1, sizeof(Token));
    head->kw = -1;
    head->value = '\n';
    head->next = NULL;
    Token *cur_token = head;
    char *ipt = input;

    int *idfd_kw;//identified keyword

    while(*ipt){
      if (isspace(*ipt)) {
        ipt++;
        continue;
      } else {
        idfd_kw = kwIdfr(ipt++);
      }

      
    }
}

Token *makeToken(int kind, Token *cur_token, char *ipt){
  Token *new_token = calloc(1, sizeof(Token));
  new_token->kw = kind;
  new_token->value = *ipt;
  cur_token->next = new_token;

  return new_token;
}

int *kwIdfr(char *ipt){//keyword identifier   
    if()
}