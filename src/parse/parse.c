#include "parse.h"

Node *parse(Token *hive_token) {
    Node *root = createNode(AST_PROGRAM);
    Node *hive = root;

    Token *cur = hive_token;
    while (cur && cur->kind != EOT) {
        root->left = functionNode(&cur);
        root->right = createNode(AST_PROGRAM);
        root = root->right;
    }

    return hive;
}