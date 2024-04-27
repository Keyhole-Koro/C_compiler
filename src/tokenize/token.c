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

const int size_single_char = sizeof(single_char) / sizeof(single_char[0]);
const int size_keywords = sizeof(keywords) / sizeof(keywords[0]);
const int size_operators = sizeof(operators) / sizeof(operators[0]);

/** @brief find the corresponding Token from keyvalue declared in token.h **/
symbol findTokenKind(char *key) {
    void *target = NULL;
    int size = 0;
    
    if (strlen(key) == 1) {
      target = single_char;
      size = size_single_char;
    } else if (isAlphabet(key)) {
      target = keywords;
      size = size_keywords;
    } else {
      target = operators;
      size = size_operators;
    }

    for (int i = 0; i < size; i++) {
        char *keyStr = ((struct KeyValue *)target)[i].Key;
        if (strcmp(keyStr, key) == 0) return ((struct KeyValue *)target)[i].Value;
    }
    return -1;
}

typedef struct SetKeyValue SetKeyValue;
struct SetKeyValue{
    struct KeyValue *target;
    int size;
    struct SetKeyValue *next;
};

SetKeyValue skv = 
  (SetKeyValue){(struct KeyValue*)&single_char, size_single_char, 
  &((SetKeyValue){(struct KeyValue*)&keywords, size_keywords,
  &((SetKeyValue){(struct KeyValue*)&operators, size_operators, NULL})})};

char *revertToken(Token *tk) {
  symbol sym = tk->kind;
    for (SetKeyValue *kv = &skv; kv; kv = kv->next) {
        for (int j = 0; j < kv->size; j++) {
            symbol value = kv->target[j].Value;
            if (value == sym) return kv->target[j].Key;
        }
    }
    if (tk->value) return tk->value;
    return NULL;
}