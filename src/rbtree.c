#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/rbtree.h"

Node *TNULL;
Node *root = NULL;

Node* createNode(char *word) {
    Node *node = (Node *)malloc(sizeof(Node));
    strncpy(node->word, word, MAX_WORD_LEN);
    node->word[MAX_WORD_LEN] = '\0';
    node->parent = TNULL;
    node->left = TNULL;
    node->right = TNULL;
    node->color = RED;
    node->synonym_count = 0;
    for (int i = 0; i < MAX_SYNONYMS; i++) {
        node->synonyms[i] = NULL;
    }
    return node;
}

void initializeTree() {
    TNULL = (Node *)malloc(sizeof(Node));
    TNULL->color = BLACK;
    TNULL->left = TNULL->right = TNULL->parent = NULL;
    TNULL->synonym_count = 0;
    root = TNULL;
}

void leftRotate(Node *x) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left != TNULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == TNULL) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void rightRotate(Node *x) {
    Node *y = x->left;
    x->left = y->right;
    if (y->right != TNULL) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == TNULL) {
        root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

void insertFixup(Node *k) {
    Node *u;
    while (k->parent->color == RED) {
        if (k->parent == k->parent->parent->right) {
            u = k->parent->parent->left;
            if (u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->left) {
                    k = k->parent;
                    rightRotate(k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                leftRotate(k->parent->parent);
            }
        } else {
            u = k->parent->parent->right;
            if (u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->right) {
                    k = k->parent;
                    leftRotate(k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                rightRotate(k->parent->parent);
            }
        }
        if (k == root) break;
    }
    root->color = BLACK;
}

void insert(char *word) {
    if (search(root, word) != TNULL) {
        printf("Слово '%s' уже существует в словаре.\n", word);
        return;
    }

    Node *node = createNode(word);
    Node *y = TNULL;
    Node *x = root;

    while (x != TNULL) {
        y = x;
        if (strcmp(node->word, x->word) < 0) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    node->parent = y;
    if (y == TNULL) {
        root = node;
    } else if (strcmp(node->word, y->word) < 0) {
        y->left = node;
    } else {
        y->right = node;
    }

    if (node->parent == TNULL) {
        node->color = BLACK;
        return;
    }
    if (node->parent->parent == TNULL) {
        return;
    }

    insertFixup(node);
    printf("Слово '%s' добавлено.\n", word);
}

void rbTransplant(Node *u, Node *v) {
    if (u->parent == TNULL) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

Node* minimum(Node *node) {
    while (node->left != TNULL) {
        node = node->left;
    }
    return node;
}

void deleteFixup(Node *x) {
    Node *s;
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            s = x->parent->right;
            if (s->color == RED) {
                s->color = BLACK;
                x->parent->color = RED;
                leftRotate(x->parent);
                s = x->parent->right;
            }
            if (s->left->color == BLACK && s->right->color == BLACK) {
                s->color = RED;
                x = x->parent;
            } else {
                if (s->right->color == BLACK) {
                    s->left->color = BLACK;
                    s->color = RED;
                    rightRotate(s);
                    s = x->parent->right;
                }
                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->right->color = BLACK;
                leftRotate(x->parent);
                x = root;
            }
        } else {
            s = x->parent->left;
            if (s->color == RED) {
                s->color = BLACK;
                x->parent->color = RED;
                rightRotate(x->parent);
                s = x->parent->left;
            }
            if (s->right->color == BLACK && s->left->color == BLACK) {
                s->color = RED;
                x = x->parent;
            } else {
                if (s->left->color == BLACK) {
                    s->right->color = BLACK;
                    s->color = RED;
                    leftRotate(s);
                    s = x->parent->left;
                }
                s->color = x->parent->color;
                x->parent->color = BLACK;
                s->left->color = BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

void cleanSynonymReferences(Node *node, Node *target) {
    if (node == TNULL) return;
    
    int found = 0;
    for (int i = 0; i < node->synonym_count; i++) {
        if (node->synonyms[i] == target) {
            for (int j = i; j < node->synonym_count - 1; j++) {
                node->synonyms[j] = node->synonyms[j+1];
            }
            node->synonym_count--;
            i--; 
            found = 1;
        }
    }
    cleanSynonymReferences(node->left, target);
    cleanSynonymReferences(node->right, target);
}

void cleanAllReferencesTo(Node *target) {
    cleanSynonymReferences(root, target);
}

void deleteNodeHelper(Node *node, char *word) {
    Node *z = TNULL;
    Node *x, *y;
    
    Node *curr = node;
    while (curr != TNULL) {
        if (strcmp(word, curr->word) == 0) {
            z = curr;
            break;
        }
        if (strcmp(word, curr->word) < 0) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }

    if (z == TNULL) {
        printf("Слово '%s' не найдено для удаления.\n", word);
        return;
    }

    cleanAllReferencesTo(z);

    y = z;
    Color y_original_color = y->color;
    if (z->left == TNULL) {
        x = z->right;
        rbTransplant(z, z->right);
    } else if (z->right == TNULL) {
        x = z->left;
        rbTransplant(z, z->left);
    } else {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            rbTransplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        rbTransplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    
    free(z);
    
    if (y_original_color == BLACK) {
        deleteFixup(x);
    }
    printf("Слово '%s' удалено.\n", word);
}

void deleteWord(char *word) {
    deleteNodeHelper(root, word);
}

Node* search(Node *root, char *word) {
    if (root == TNULL || strcmp(word, root->word) == 0) {
        return root;
    }
    if (strcmp(word, root->word) < 0) {
        return search(root->left, word);
    }
    return search(root->right, word);
}

void printTreeHelper(Node *root, char *indent, int last) {
    if (root != TNULL) {
        printf("%s", indent);
        if (last) {
            printf("R----");
            strcat(indent, "   ");
        } else {
            printf("L----");
            strcat(indent, "|  ");
        }
        char colorChar = root->color == RED ? 'R' : 'B';
        printf("%s(%c)\n", root->word, colorChar);
        printTreeHelper(root->left, indent, 0);
        printTreeHelper(root->right, indent, 1);
        indent[strlen(indent)-3] = '\0';
    }
}

void printTree() {
    char indent[1000] = ""; 
    if (root == TNULL) {
        printf("Дерево пусто.\n");
    } else {
        printTreeHelper(root, indent, 1);
    }
}