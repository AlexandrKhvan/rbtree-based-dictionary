#ifndef RBTREE_H
#define RBTREE_H

#include <stdio.h>

#define MAX_WORD_LEN 30
#define MAX_SYNONYMS 10

typedef enum { RED, BLACK } Color;

typedef struct Node {
    char word[MAX_WORD_LEN + 1];
    Color color;
    struct Node *left, *right, *parent;
    struct Node *synonyms[MAX_SYNONYMS];
    int synonym_count;
} Node;

extern Node *TNULL;
extern Node *root;

void initializeTree();
void insert(char *word);
void deleteWord(char *word);
Node* search(Node *root, char *word);
void printTree();

Node* createNode(char *word);
Node* minimum(Node *node);
void cleanAllReferencesTo(Node *target);

#endif // RBTREE_H