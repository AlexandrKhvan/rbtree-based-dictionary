#ifndef DICT_H
#define DICT_H

#include <stdio.h>
#include "rbtree.h"

#define FILENAME "dictionary.txt"

void addSynonym(char *word1, char *word2);

void searchByPrefix(Node *node, char *prefix, int *found);
void renameWord(char *oldWord, char *newWord);

void printNodeInfo(Node *node);

void saveToFile();
void loadFromFile();

#endif // DICT_H