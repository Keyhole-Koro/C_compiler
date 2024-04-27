#include "parse.h"

Node *parse(Token *hive_token) {
    Node *root = NULL;

    for (Token *cur = hive_token; cur; cur = cur->next) {
        TokenKind kind = cur->kind;

        if (kind == NUMBER || kind == L_PARENTHESES) {
            root = expr(&cur);
        }

    }

    return root;
}