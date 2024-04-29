#include "AST.h"

Node *createNode_(AST_Type type, ValueKind valKind, Value value);

Node *createNaturalNode(AST_Type type, int value) {
    Value v;
    v.natural = value;
    return createNode_(type, NATURAL_NUMBER, v);
}

Node *createDecimalNode(AST_Type type, float value) {
    Value v;
    v.decimal = value;
    return createNode_(type, DECIMAL_FRACTION, v);
}

Node *createStringNode(AST_Type type, char *value) {
    Value v;
    v.str = value;
    return createNode_(type, STRING, v);
}

Node *createNode(AST_Type type) {
    Node *newNode = calloc(1, sizeof(Node));
    newNode->type = type;
    newNode->valKind = NOTHING;
    return newNode;
}

Node *createNode_(AST_Type type, ValueKind valKind, Value value) {
    Node *newNode = calloc(1, sizeof(Node));
    newNode->type = type;
    newNode->valKind = valKind;
    newNode->value = value;
    return newNode;
}

void printPreorder(Node* node, int depth, char branch) {
    if (node == NULL)
        return;
    
    switch (node->valKind) {
        case NATURAL_NUMBER:
            DEBUG_PRINT("%*s%c-- %d\n", depth * 4, "", branch, node->value.natural);
            break;
        case DECIMAL_FRACTION:
            DEBUG_PRINT("%*s%c-- %f\n", depth * 4, "", branch, node->value.decimal);
            break;
        case STRING:
            DEBUG_PRINT("%*s%c-- %s\n", depth * 4, "", branch, node->value.str);
            break;
        default:
            DEBUG_PRINT("%*s%c-- \n", depth * 4, "", branch);
            break;
    }


    printPreorder(node->left, depth + 1, '|');

    printPreorder(node->right, depth + 1, '`');
}