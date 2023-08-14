#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node Node;

struct Node {
    int value;
    char *symbol;
    Node *left;
    Node *below;
    Node *right;
};

int asciiToNumber(char character) {
    return (int)character;
}

int convertAsciiAtOnce(char *ascii) {
    size_t length = strlen(ascii);
    int sum_num = 0;
    for (size_t i = 0; i < length; i++) {
        sum_num += asciiToNumber(ascii[i]);
    }
    return sum_num;
}

Node *createNode(int value, char *symbol) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Memory allocation failed for new node");
        exit(1);
    }
    newNode->value = value;
    newNode->symbol = symbol;
    newNode->left = NULL;
    newNode->below = NULL;
    newNode->right = NULL;
    return newNode;
}

Node *insert(Node *root, int value, char *symbol) {
    if (root == NULL) return createNode(value, symbol);

    if (value < root->value) {
        root->left = insert(root->left, value, symbol);
    } else if (value == root->value) {
        root->below = insert(root->below, value, symbol);
    } else {
        root->right = insert(root->right, value, symbol);
    }

    return root;
}

void inorderTraversal(Node *root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("(%d, %s) ", root->value, root->symbol);
        inorderTraversal(root->right);
    }
}

int main() {
    char ascii[] = "abcde";
    int sum = convertAsciiAtOnce(ascii);
    printf("Sum of ASCII values: %d\n", sum);

    Node *root = NULL;
    root = insert(root, sum, "abcde");

    printf("Inorder traversal: ");
    inorderTraversal(root);
    printf("\n");

    return 0;
}
