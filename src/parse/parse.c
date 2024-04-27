#include "parse.h"

void parse(Token *hive_token) {
    for (Token *cur = hive_token; cur->next; cur = cur->next) {
        TokenKind kind = cur->kind;

        if (kind == NUMBER || kind == L_PARENTHESES) {
            Node *newNode = expr(&cur);
        }
    }
}