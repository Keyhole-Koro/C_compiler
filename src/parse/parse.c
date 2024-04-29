#include "parse.h"

Node *parse(Token *hive_token) {
    Node *root = NULL;

    for (Token *cur = hive_token; cur; cur = cur->next) {
        root = statementNode(&cur); 
    }

    return root;
}