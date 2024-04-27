#include "parse.h"

void parse(Token *hive_token) {
    for (Token *cur = hive_token; cur->next; cur = cur->next) {
        TokenKind kind = cur->kind;

        if (kind == NUMBER || L_PARENTHESES) {
            Node *node = NULL;
            Token *rest = expr(cur, node);
        }
    }
}



