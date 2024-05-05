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
    if (newNode == NULL) {
        DEBUG_PRINT("failed to allocate\n");
        exit(1);
    }
    newNode->type = type;
    newNode->valKind = NOTHING;
    return newNode;
}

Node *createNode_(AST_Type type, ValueKind valKind, Value value) {
    Node *newNode = calloc(1, sizeof(Node));
    if (newNode == NULL) {
        DEBUG_PRINT("failed to allocate\n");
        exit(1);
    }
    newNode->type = type;
    newNode->valKind = valKind;
    newNode->value = value;
    return newNode;
}

#include <string.h> // Include string.h for strcpy

char str_buf[20];

void printPreorder(Node* node, int depth, char branch) {
    if (node == NULL) return;
        
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
            switch (node->type) {
                case AST_AND:
                    strcpy(str_buf, "&&");
                    break;
                case AST_OR:
                    strcpy(str_buf, "||");
                    break;
                case AST_EQ:
                    strcpy(str_buf, "==");
                    break;
                case AST_NEQ:
                    strcpy(str_buf, "!=");
                    break;
                case AST_GT:
                    strcpy(str_buf, ">");
                    break;
                case AST_GTE:
                    strcpy(str_buf, ">=");
                    break;
                case AST_LT:
                    strcpy(str_buf, "<");
                    break;
                case AST_LTE:
                    strcpy(str_buf, "<=");
                    break;
                default:
                    strcpy(str_buf, "_");
                    break;
            }
            DEBUG_PRINT("%*s%c-- %s\n", depth * 4, "", branch, str_buf);
            break;
    }


    printPreorder(node->left, depth + 1, '|');

    printPreorder(node->right, depth + 1, '`');
}

int getNodeType(Node *node) {
    return node->type;
}