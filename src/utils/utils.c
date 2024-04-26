#include "utils.h"

inline void error(char ch[]) {
    fprintf(stderr, "%s", ch);
    exit(1);
}

char *readWhile(bool (*condition)(char*), char *ipt, char **rest) {
    size_t buffer_size = 8;

    char *buffer = (char *)malloc(sizeof(char) * buffer_size);

    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    *buffer = '\0';

    size_t buffer_length = 0;

    while (*ipt && condition(ipt)) {

        if (*ipt == ' ') {
            ipt++;
            continue;
        }

        if (buffer_length > buffer_size) {
            buffer_size *= 2;
            buffer = (char *)realloc(buffer, buffer_size * sizeof(char));
            
            if (buffer == NULL) {
                fprintf(stderr, "Memory reallocation failed\n");
                exit(EXIT_FAILURE);
            }
        }

        buffer[buffer_length++] = *ipt++;
    }

    buffer[buffer_length] = '\0';

    *rest = ipt;

    return buffer;
}

inline bool isEnd(char *letter) {
  return (*letter == '\n');
}

inline bool isTab(char *letter) {
  return (*letter == '\t');
}

inline bool isEndComment(char *letter) {
  return (*letter == '*' && *(letter + 1) == '/');
}

inline bool isAlphabet(char *letter) {
  return ((*letter >= 'a' && *letter <= 'z') || (*letter >= 'A' && *letter <= 'Z'));
}

inline bool isNumber(char *letter) {
  return (*letter >= '0' && *letter <= '9');
}

inline bool isAlphabet_Num(char *letter) {
  return (isAlphabet(letter) || isNumber(letter));
}

inline bool isAlphabet_Num_Underbar(char *letter) {
  return (isAlphabet_Num(letter) || *letter == '_');
}

inline bool isAlphabet_Underbar(char *letter) {
  return (isAlphabet(letter) || *letter == '_');
}

inline bool isNOTAlphabet_Num_Underbar(char *letter) {
  return !isAlphabet_Num_Underbar(letter);
}
