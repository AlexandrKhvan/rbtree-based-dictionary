#include <stdio.h>
#include <stdlib.h>
#include "include/rbtree.h"
#include "include/dict.h"

void printMenu() {
    printf("\n--- Меню Словаря ---\n");
    printf("1. Добавить слово\n");
    printf("2. Найти слово\n");
    printf("3. Удалить слово\n");
    printf("4. Добавить синоним\n");
    printf("5. Найти по префиксу\n");
    printf("6. Переименовать слово\n");
    printf("7. Показать дерево (Debug)\n");
    printf("8. Сохранить в файл\n");
    printf("9. Загрузить из файла\n");
    printf("0. Выход\n");
    printf("Выбор: ");
}

int main() {
    initializeTree();
    int choice;
    char buffer1[100], buffer2[100];
    
    // loadFromFile(); // Опциональная автозагрузка

    while (1) {
        printMenu();
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                printf("Введите слово: ");
                scanf("%s", buffer1);
                insert(buffer1);
                break;
            case 2:
                printf("Введите слово для поиска: ");
                scanf("%s", buffer1);
                Node *res = search(root, buffer1);
                printNodeInfo(res);
                break;
            case 3:
                printf("Введите слово для удаления: ");
                scanf("%s", buffer1);
                deleteWord(buffer1);
                break;
            case 4:
                printf("Введите первое слово: ");
                scanf("%s", buffer1);
                printf("Введите синоним: ");
                scanf("%s", buffer2);
                addSynonym(buffer1, buffer2);
                break;
            case 5:
                printf("Введите префикс: ");
                scanf("%s", buffer1);
                int count = 0;
                printf("Результаты:\n");
                searchByPrefix(root, buffer1, &count);
                if (count == 0) printf("Ничего не найдено.\n");
                break;
            case 6:
                printf("Старое имя: ");
                scanf("%s", buffer1);
                printf("Новое имя: ");
                scanf("%s", buffer2);
                renameWord(buffer1, buffer2);
                break;
            case 7:
                printTree();
                break;
            case 8:
                saveToFile();
                break;
            case 9:
                loadFromFile();
                break;
            case 0:
                printf("Выход...\n");
                exit(0);
            default:
                printf("Неверный выбор.\n");
        }
    }
    return 0;
}