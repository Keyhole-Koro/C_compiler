#include "parse.h"

Node *parse(Token *hive_token) {
    Node *root = createNode(AST_PROGRAM);
    Node *hive = root;

    Token *cur = hive_token;
    while (cur && cur->kind != EOT) {
        root->left = functionNode(&cur);
        root = root->right;
        root = createNode(AST_PROGRAM);
    }

    return hive;
}