#ifndef TOKEN_H
#define TOKEN_H

#include "utils.h"

typedef int symbol;

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

    POINTER,  // *
    ADDRESS,  // &
    MEMBER,   // ->

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

    L_PARENTHESES, // (
    R_PARENTHESES, // )
    SEMICOLON,     // ;
    COMMA,         // ,
    L_BRACE,       // {
    R_BRACE,       // }
    L_BRACKET,     // [
    R_BRACKET,     // ]
    DOT,           // .
    QUESTION,      // ?
    COLON,         // :
    VERTICAL_BAR,  // |
    CARET,         // ^
    HASH,          // #

    DOUBLE_SLASH,  // //

    NUMBER, // number

    IDENTIFIER, // defined by user

    INLINE, // inline but not implemented

    EOT, // end of token

} TokenKind;

typedef struct Token Token;

struct Token{
  TokenKind kind;
  char *value;
  Token *next;
};

struct KeyValue {
  char *Key;
  symbol Value;
};

extern struct KeyValue operators[];
extern struct KeyValue keywords[];
extern struct KeyValue single_char[];

symbol findTokenKind(char *key);
char *revertToken(Token *tk);

#endif