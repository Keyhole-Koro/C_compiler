#include "parse.h"

Node *parse(Token *hive_token) {
    Node *root = NULL;

    for (Token *cur = hive_token; cur; cur = cur->next) {
        DEBUG_PRINT("1\n");
        statementNode(&cur); 
        DEBUG_PRINT("2\n");
    }

    return root;
}