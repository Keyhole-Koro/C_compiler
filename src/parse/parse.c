#include "parse.h"

void parse(Token *hive_token) {
    Token *cur = hive_token;

    for (Token *cur = hive_token; cur->next; cur = cur->next) {
        Token *next = cur->next;
        TokenKind kind = next->kind;

        if (kind == ADD || SUB || MUL || DIV) {
            Node *node = NULL;
            Token *rest = expr(cur, next, &node);
        }
    }
}

Node *expr(Token *prev, Token *cur, Token **root) {
    Token *next = cur->next;
    Node *new_node = NULL;
    if (cur == ADD) new_node = createNode(AST_ADD);
    if (cur == SUB) new_node = createNode(AST_SUB);
    if (cur == MUL) new_node = createNode(AST_MUL);
    if (cur == DIV) new_node = createNode(AST_DIV);
    
    if (prev->kind == NUMBER) {
        new_node->left = AST_NUMBER;
        new_node->num = 
    }
    if (!(*root)) root = newNode;
}

Node *createNode(AST_Type type) {
    Node *newNode = calloc(1, sizeof(Node));
    newNode->type = type;
    return newNode;
}