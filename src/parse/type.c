#include "type.h"

Type *isType_(Token *tk);

Type type_int = (Type){"int", 4};

Type type_void = (Type){"void", 0};

Type *isType(Token *tk) {
    Type *buf = isType_(tk);
    if (!buf) return NULL;

    Type *clone = malloc(sizeof(Type));
    memcpy(clone, buf, sizeof(Type));
    return clone;
}

Type *isType_(Token *tk) {
    Type *found = NULL;
    switch (tk->kind) {
        case BOOL:
            return NULL; //tmp
        case INT:
            return &type_int;
        case CHAR:
        case FLOAT:
        case DOUBLE:
        case VOID:
            found = NULL; //tmp
            break;
        default:
            found = NULL;
            break;
    }
    // look for self defined types

    return found;
}

Node *typeNode(Type *type) {
    Node *type_node = createStringNode(AST_TYPE, type->name);
    type_node->left = createNaturalNode(AST_TYPE_SIZE, type->size);
    return type_node;
}