#include "token.h"


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

bool ifMatch(char *target, char *pattern, size_t read_size){
  size_t pattern_length = strlen(pattern);

  if (read_size > pattern_length) return false;

	if (strncmp(target, pattern, read_size) == 0) return true;
	return false;
}

/** @brief find the corresponding Token from keyvalue declared in token.h **/
int findCorrespondToken(char *substring, size_t substring_length, struct KeyValue *symbols) {
  size_t size = sizeof(symbols) / sizeof(symbols[0]);
  for (int i = 0; i < size; i++) {
    char *key = symbols[i].Key;
    if (ifMatch(substring, key, substring_length)) {
        return symbols[i].Value;
    }
  }
  return -1;
}