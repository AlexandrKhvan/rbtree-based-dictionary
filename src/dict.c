#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/dict.h"
#include "../include/rbtree.h"

void addSynonym(char *word1, char *word2) {
    Node *n1 = search(root, word1);
    Node *n2 = search(root, word2);

    if (n1 == TNULL || n2 == TNULL) {
        printf("Ошибка: Одно из слов не найдено.\n");
        return;
    }
    if (n1 == n2) {
        printf("Нельзя добавить слово как синоним самого себя.\n");
        return;
    }

    if (n1->synonym_count < MAX_SYNONYMS) {
        int exists = 0;
        for(int i=0; i<n1->synonym_count; i++) {
            if(n1->synonyms[i] == n2) exists = 1;
        }
        if(!exists) {
            n1->synonyms[n1->synonym_count++] = n2;
            printf("'%s' добавлен как синоним к '%s'.\n", word2, word1);
        } else {
             printf("Связь уже существует.\n");
        }
    } else {
        printf("Лимит синонимов для '%s' исчерпан.\n", word1);
    }

    if (n2->synonym_count < MAX_SYNONYMS) {
        int exists = 0;
        for(int i=0; i<n2->synonym_count; i++) {
            if(n2->synonyms[i] == n1) exists = 1;
        }
        if(!exists) {
            n2->synonyms[n2->synonym_count++] = n1;
            printf("'%s' добавлен как синоним к '%s'.\n", word1, word2);
        }
    }
}

void searchByPrefix(Node *node, char *prefix, int *found) {
    if (node == TNULL) return;
    
    searchByPrefix(node->left, prefix, found);
    
    if (strncmp(node->word, prefix, strlen(prefix)) == 0) {
        printf("- %s\n", node->word);
        (*found)++;
    }
    
    searchByPrefix(node->right, prefix, found);
}

void renameWord(char *oldWord, char *newWord) {
    Node *oldNode = search(root, oldWord);
    if (oldNode == TNULL) {
        printf("Слово '%s' не найдено.\n", oldWord);
        return;
    }
    
    if (search(root, newWord) != TNULL) {
        printf("Слово '%s' уже существует. Используйте слияние или удаление.\n", newWord);
        return;
    }

    Node *savedSynonyms[MAX_SYNONYMS];
    int savedCount = oldNode->synonym_count;
    for (int i = 0; i < savedCount; i++) {
        savedSynonyms[i] = oldNode->synonyms[i];
    }

    deleteWord(oldWord);
    insert(newWord);
    Node *newNode = search(root, newWord);

    for (int i = 0; i < savedCount; i++) {
        if (savedSynonyms[i] != NULL) {
             newNode->synonyms[newNode->synonym_count++] = savedSynonyms[i];
             Node *syn = savedSynonyms[i];
             if (syn->synonym_count < MAX_SYNONYMS) {
                 syn->synonyms[syn->synonym_count++] = newNode;
             }
        }
    }
    printf("Переименование завершено.\n");
}

void printNodeInfo(Node *node) {
    if (node == TNULL) {
        printf("Слово не найдено.\n");
        return;
    }
    printf("Слово: %s\n", node->word);
    printf("Синонимы: ");
    if (node->synonym_count == 0) {
        printf("(нет)");
    } else {
        for (int i = 0; i < node->synonym_count; i++) {
            printf("%s", node->synonyms[i]->word);
            if (i < node->synonym_count - 1) printf(", ");
        }
    }
    printf("\n");
}

void saveNodeHelper(FILE *fp, Node *node) {
    if (node == TNULL) return;
    saveNodeHelper(fp, node->left);
    fprintf(fp, "%s", node->word);
    for (int i = 0; i < node->synonym_count; i++) {
        fprintf(fp, " %s", node->synonyms[i]->word);
    }
    fprintf(fp, "\n");
    saveNodeHelper(fp, node->right);
}

void saveToFile() {
    FILE *fp = fopen(FILENAME, "w");
    if (!fp) {
        perror("Ошибка открытия файла");
        return;
    }
    saveNodeHelper(fp, root);
    fclose(fp);
    printf("Словарь сохранен в %s\n", FILENAME);
}

void loadFromFile() {
    FILE *fp = fopen(FILENAME, "r");
    if (!fp) {
        printf("Файл %s не найден, создан новый словарь.\n", FILENAME);
        return;
    }
    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, " \n\r");
        if (token) {
            insert(token);
        }
    }
    rewind(fp);
    while (fgets(line, sizeof(line), fp)) {
        char *mainWord = strtok(line, " \n\r");
        if (!mainWord) continue;
        char *synWord;
        while ((synWord = strtok(NULL, " \n\r")) != NULL) {
            addSynonym(mainWord, synWord); 
        }
    }
    fclose(fp);
    printf("Словарь загружен.\n");
}