#ifndef TOKEN_H
#define TOKEN_H

#include "utils.h"

typedef int symbol_s;

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

    INLINE // inline but not implemented
} TokenKind;

typedef struct {
  TokenKind Tk;
  char value;
} Token;

struct KeyValue {
  char *Key;
  int Value;
};

struct KeyValue operators[] = {
  {"==", EQ},
  {"!=", NEQ},
  {"<=", LTE},
  {">=", GTE},
  {"&&", AND},
  {"||", OR},  
  {"<<", SHL},
  {">>", SHR},
  {"++", INC},
  {"--", DEC},
  {"*", POINTER},
  {"->", MEMBER},
};

struct KeyValue keywords[] = {
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
};

struct KeyValue single_char[] = {
  {"+", ADD},
  {"-", SUB},
  {"*", MUL},
  {"/", DIV},
  {"%", MOD},
  {"=", ASSIGN},
  {"(", L_PARENTHESES},
  {")", R_PARENTHESES},
  {";", SEMICOLON},
  {",", COMMA},
  {"{", L_BRACE},
  {"}", R_BRACE},
  {"[", L_BRACKET},
  {"]", R_BRACKET},
  {"<", LT},
  {">", GT},
  {".", DOT},
  {"!", NOT},
  {"?", QUESTION},
  {":", COLON},
  {"|", BITOR},
  {"^", BITXOR},
  {"~", BITNOT},
  {"#", HASH},
  {"&", BITAND},
};

int findCorrespondToken(char *substring, size_t substring_length, struct KeyValue *symbols);

#endif