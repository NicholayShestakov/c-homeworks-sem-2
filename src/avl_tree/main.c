#include "avl_tree.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
    AVLTree* tree = avlTreeCreate();
    if (tree == NULL) {
        errno = 12;
        perror("Ошибка при создании дерева");
        return errno;
    }

    if (argc > 1) {
        if (!avlTreeAddFromFile(tree, argv[1])) {
            errno = 14;
            perror("Ошибка при получении данных из файла");
            return errno;
        }

        size_t size = 0;
        if (!avlTreeSize(tree, &size)) {
            errno = 14;
            perror("Ошибка при обработке количества узлов в дереве");
            return errno;
        }
        printf("Загрузка прошла успешно. Количество аэропортов в базе данных: %ld\n\n", size);
    } else {
        printf("Внимание! На вход не было передано названия файла. База данных пуста.\n");
        printf("Чтобы использовать данные из файла, запустите программу с флагом <имя файла>.\n\n");
    }

    printf("Вас приветствует Aerosoft - программа для хранения базы аэропортов и выполнения операций над ней.\n");
    printf("Команды:\n");
    printf("find <код> - Найти название аэропорта по коду IATA.\n");
    printf("add <код>:<название> - Добавить новый аэропорт в базу.\n");
    printf("delete <код> - Удалить аэропорт из базы.\n");
    printf("save - Сохранить текущее состояние базы обратно в %s.\n", argv[1]);
    printf("quit - Завершить работу программы.\n");
    printf("Максимальная длина аргумента команды - 254 символа.\n\n");

    char command[8] = { 0 };
    char argument[256] = { 0 };
    while (true) {
        printf("> ");
        scanf("%7s", command);
        fgets(argument, sizeof(argument), stdin);
        argument[strcspn(argument, "\n")] = 0; // Удаление переноса строки.
        char* correctArgument = argument + 1; // Чтобы не брать начальный пробел.

        if (strcmp(command, "find") == 0) {
            char* value = avlTreeFind(tree, correctArgument);
            if (value == NULL) {
                printf("Аэропорта с кодом %s не существует. Если хотите добавить его в базу, используйте команду add.\n\n", correctArgument);
            } else {
                printf("Аэропорт с кодом %s - %s.\n\n", correctArgument, value);
            }
        } else if (strcmp(command, "add") == 0) {
            size_t colonIndex = strcspn(correctArgument, ":");
            char* valueArgument = correctArgument + colonIndex + 1;

            if (avlTreeFind(tree, correctArgument) != NULL) {
                printf("Аэропорт с ключом %s уже существует. Если хотите изменить его, воспользуйтесь командой delete, а затем add.\n\n", correctArgument);
                continue;
            }
            if (correctArgument[colonIndex] != ':') {
                printf("Не обнаружено разделяющего двоеточия. Используйте формат add <код>:<название>.\n\n");
                continue;
            }
            if (valueArgument[strcspn(valueArgument, ":")] == ':') {
                printf("Использование двоеточия в названии не разрешено. Попробуйте заменить его на что-нибудь другое.\n\n");
                continue;
            }
            correctArgument[colonIndex] = 0; // Чтобы считывал только ключ.
            if (avlTreeAdd(tree, correctArgument, valueArgument)) {
                printf("Аэропорт %s с кодом %s успешно добавлен в базу данных.\n\n", valueArgument, correctArgument);
            } else {
                printf("Что-то пошло не так. Попробуйте ещё раз. Если проблема сохранится, можете выбить из создателя компенсацию морального ущерба.\n\n");
            }
        } else if (strcmp(command, "delete") == 0) {
            if (avlTreeFind(tree, correctArgument) == NULL) {
                printf("Аэропорта с ключом %s не существует.\n\n", correctArgument);
            } else {
                if (avlTreeDelete(tree, correctArgument)) {
                    printf("Аэропорт с кодом %s был успешно удалён.\n\n", correctArgument);
                } else {
                    printf("Что-то пошло не так. Попробуйте ещё раз. Если проблема сохранится, можете выбить из создателя компенсацию морального ущерба.\n\n");
                }
            }
        } else if (strcmp(command, "save") == 0) {
            if (avlTreeSaveInFile(tree, argv[1])) {
                printf("Сохранение данных в файл %s прошло успешно.\n\n", argv[1]);
            } else {
                printf("Что-то пошло не так. Попробуйте ещё раз. Если проблема сохранится, можете выбить из создателя компенсацию морального ущерба.\n\n");
            }
        } else if (strcmp(command, "quit") == 0) {
            printf("Работа программы завершена.\n");
            break;
        } else {
            printf("Команды не существует.\n\n");
        }
    }

    avlTreeFree(&tree);
    return 0;
}
