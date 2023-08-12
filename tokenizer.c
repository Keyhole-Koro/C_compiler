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

    L_PARENTHESES, // (
    R_PARENTHESES, // )
    SEMICOLON,     // ;
    COMMA,         // ,
    L_BRACE,       // {
    R_BRACE,       // }
    L_BRACKET,     // [
    R_BRACKET,     // ]
    L_ANGLE,       // <
    R_ANGLE,       // >
    AMPERSAND,     // &
    DOT,           // .
    EXCLAMATION,   // !
    QUESTION,      // ?
    COLON,         // :
    VERTICAL_BAR,  // |
    CARET,         // ^
    PERCENT,       // %
    TILDE,         // ~
    HASH,          // #

    VARIABLE, //defined by user
};

typedef struct Token Token;

typedef struct {
  Token *token;
  Buffer *buffer_idx;
  int *data;
  size_t capacity;
  size_t size;
  char *file_path;
} Vector;

typedef struct {
  int tk;
  //add other fields later
} Token;

typedef struct {
  char *start_idx;
  char *end_idx;
} Buffer;

struct KeyValue {
  char *Key;
  int Value;
};

struct KeyValue operators[] = {
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
  {"variable", VARIABLE},
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
  {"<", L_ANGLE},
  {">", R_ANGLE},
  {"&", AMPERSAND},
  {".", DOT},
  {"!", EXCLAMATION},
  {"?", QUESTION},
  {":", COLON},
  {"|", VERTICAL_BAR},
  {"^", CARET},
  {"~", TILDE},
  {"#", HASH},
};

bool isIndentation(char *letter);
bool isAlphabet_Num_Underbar(char *letter);
bool isAlphabet(char *letter);
bool isAlphabet_Underbar(char *letter);
bool isAlphabet_Num(char *letter);

Vector *createVector() {
  Vector *vec = (Vector *)malloc(sizeof(Vector));
  if (vec == NULL) {
    perror("Failed to allocate memory for vector");
    exit(EXIT_FAILURE);
  }
  vec->data = NULL;
  vec->size = 0;
  vec->capacity = 0;
  return vec;
}

void pushBack(Vector *vec, int value) {
  if (vec->size >= vec->capacity) {
    size_t newCapacity = (vec->capacity == 0) ? 1 : vec->capacity * 2;
    int *newData = (int *)realloc(vec->token, newCapacity * sizeof(int));
    if (newData == NULL) {
        perror("Failed to reallocate memory for vector");
        exit(EXIT_FAILURE);
    }
    vec->data = newData;
    vec->capacity = newCapacity;
  }
  vec->data[vec->size++] = value;
}

Token *makeToken(int kind, char *ipt){
  Token *new_token = calloc(1, sizeof(Token));
  new_token->tk = kind;
  return new_token;
}

Buffer *makeBuffer(char *start_idx, char *end_idx){
  Buffer *new_buffer = calloc(1, sizeof(Buffer));
  new_buffer->start_idx = start_idx;
  new_buffer->end_idx = end_idx;
  return new_buffer;
}

Token *tokenize(char *input) {
  Vector *vec = createVector();

  char *ipt = input;
  char *ipt_copy;
  char *end_target;
  size_t length_target;
  int idfd_vl;//identified value

  while (*ipt) {
    if (isspace(*ipt)) {
      ipt++;
      continue;
    }

    if (isIndentation(ipt)) {//consider indentation token
      ipt += 2;
      continue;
    }

    if (*ipt == '/' && *(ipt + 1) == '/') {
      end_target = readNextUntil(isIndentation, ipt);
      ipt = ++end_target;
      continue;
    }

    if (*ipt == '/' && *(ipt + 1) == '*') {
      end_target = readNextUntil(isEndComment, ipt);
      ipt = ++end_target;
      continue;
    }

    if (strchr("+-*/;=(){},<>[]&.!?:|^%~#", *ipt)) {
      idfd_vl = getValue(ipt++, 1, single_char);
      continue;
    }

    if (isAlphabet_Underbar(ipt)) {
      end_target = readNextUntil(isAlphabet_Num_Underbar, ipt);
      idfd_vl = identifier(ipt, end_target - ipt, keywords, isAlphabet_Num_Underbar);
      ipt = ++end_target;
      continue;
    }
    
    if (!isAlphabet_Num_Underbar(ipt)) {
      end_target = readNextUntil(!isAlphabet_Num_Underbar, ipt);
      idfd_vl = identifier(ipt, end_target - ipt, operators, !isAlphabet_Num_Underbar);
      ipt = ++end_target;
      continue;
    }
  }
}

bool isEndComment(char *letter) {
  if (*letter == '*' && *(letter + 1) == '/') return true;
  return false;
}

bool isIndentation(char *letter) {
  if (*letter == '\\' && *(letter + 1) == 'n') return true;
  return false;
}

bool isAlphabet_Num_Underbar(char *letter) {
	if (isAlphabet_Num(letter) || *letter == '_') return true;
	return false;
}

bool isAlphabet(char *letter) {
	if ((*letter < 'a' || *letter > 'z') && (*letter < 'A' || *letter > 'Z')) return true;
	return false;
}

bool isAlphabet_Underbar(char *letter) {
	if (isAlphabet(letter) && *letter == '_') return true;
	return false;
}

bool isAlphabet_Num(char *letter) {
	if (isAlphabet(letter) && (*letter < '0' || *letter > '9')) return true;
	return false;
}

char *readNextUntil(bool (*condition)(char), char *ptr_letter) {
  ptr_letter++;
  if (!condition(*ptr_letter)) {
    ptr_letter--;
    return ptr_letter;
  } else {
    return readNextUntil(condition, ptr_letter);
  }
}


bool ifMatch(char *target, char *pattern, size_t read_size){
  size_t pattern_length = strlen(pattern);

  if (read_size > pattern_length) return false;
	
	if (strncmp(target, pattern, read_size) == 0) return true;
	return false;
}

int identifier(char *ipt, size_t length_target, struct KeyValue *symbols, bool (*condition)(char)) {
  int value = getValue(ipt, length_target, symbols);

  if (value != -1) return value;

  return VARIABLE; // Default to VARIABLE if no keyword/operator is matched
}

int getValue(char *substring, size_t substring_length, struct KeyValue *symbols) {
  size_t size = sizeof(symbols) / sizeof(symbols[0]);
  for (int i = 0; i < size; i++) {
    char *key = symbols[i].Key;
    if (ifMatch(substring, key, substring_length)) {
        return symbols[i].Value;
    }
  }
  return -1;
}

int main() {
    // Test your tokenize function here
    char input[] = "int main() { return 0; }";
    Token *tokens = tokenize(input);
    
    // Print the tokens or perform other actions
    
    // Don't forget to free the memory allocated for tokens when done
    // ...
    
    return 0;
}