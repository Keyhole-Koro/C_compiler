#include "token.h"

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