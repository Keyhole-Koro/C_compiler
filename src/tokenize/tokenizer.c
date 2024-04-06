#include "tokenizer.h"

bool isEndComment(char *letter);
bool isIndentation(char *letter);
bool isAlphabet_Num_Underbar(char *letter);
bool isAlphabet(char *letter);
bool isAlphabet_Underbar(char *letter);
bool isAlphabet_Num(char *letter);

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

DynamicArray *tokenize(char *input) {
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

    if (strchr("+-*/;=(){},<>[]&.!?:|^%~#", *ipt) && !strchr("+-<>=&|", *ipt)) {
      idfd_vl = getValue(ipt++, 1, single_char);
      continue;
    }

    if (isAlphabet_Underbar(ipt)) {
      end_target = readNextUntil(isAlphabet_Num_Underbar, ipt);
      idfd_vl = identifier(ipt, end_target, keywords, isAlphabet_Num_Underbar);
      ipt = ++end_target;
      continue;
    }
    
    if (!isAlphabet_Num_Underbar(ipt)) {
      end_target = readNextUntil(!isAlphabet_Num_Underbar, ipt);
      untilMatch:
      idfd_vl = identifier(ipt, end_target, operators, !isAlphabet_Num_Underbar);
      if (idfd_vl == NULL) {//when it couldnt find
        end_target--;
        goto untilMatch;
      }
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

int identifier(char *ipt, char *end_target, struct KeyValue *symbols, bool (*condition)(char)) {
  if (end_target >= ipt) return NULL;

  size_t length_target = end_target - ipt;

  int value = getValue(ipt, length_target, symbols);

  if (value != -1) return value;

  return NULL;
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

void initialize() {

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