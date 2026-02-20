#include "csv2txt.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t max(size_t a, size_t b)
{
    if (a > b) {
        return a;
    }
    return b;
}

bool isNumber(char* string)
{
    bool isFloat = false;
    bool isNegative = false;
    size_t length = strlen(string);

    for (size_t i = 0; i < length; ++i) {
        switch (string[i]) {
        case '-':
            if (i != 0) {
                return false;
            }
            isNegative = true;
            break;
        case '.':
            if (i == 0 || i == (length - 1) || isFloat || (i == 1 && isNegative)) { // Случаи: .123 123. 1.23. -.123
                return false;
            }
            isFloat = true;
            break;
        default:
            if (!isdigit(string[i])) {
                return false;
            }
            break;
        }
    }

    return true;
}

void table2txt(char*** table, size_t* maxWordLength, int rowCount, int columnCount)
{
    FILE* output = fopen("output.txt", "w");

    for (int row = 0; row < rowCount; ++row) {
        // Рамочка
        for (int col = 0; col < columnCount; ++col) {
            fprintf(output, "+");
            for (size_t frameCounter = 0; frameCounter < maxWordLength[col] + 2; ++frameCounter) {
                if (row < 2) {
                    fprintf(output, "=");
                } else {
                    fprintf(output, "-");
                }
            }
        }
        fprintf(output, "+\n");
        // Значения и боковые кусочки рамочки
        for (int col = 0; col < columnCount; ++col) {
            if (isNumber(table[row][col]) && row != 0) {
                fprintf(output, "| %*s ", (int)maxWordLength[col], table[row][col]);
            } else {
                fprintf(output, "| %-*s ", (int)maxWordLength[col], table[row][col]);
            }
        }
        fprintf(output, "|\n");
    }
    // Рамочка
    for (int col = 0; col < columnCount; ++col) {
        fprintf(output, "+");
        for (size_t frameCounter = 0; frameCounter < maxWordLength[col] + 2; ++frameCounter) {
            if (rowCount < 2) {
                fprintf(output, "=");
            } else {
                fprintf(output, "-");
            }
        }
    }
    fprintf(output, "+\n");

    fclose(output);
}

void csv2txt(FILE* csv)
{
    // В некоторых динамических массивах мы увеличиваем размер на 1 а не умножаем на 2, чтобы контролировать мусор.
    char*** table = malloc(sizeof(*table));
    int tableSize = 1;
    int tableUsed = 0;
    char** row = malloc(sizeof(*row));
    int rowSize = 1;
    int rowUsed = 0;
    char* word = calloc(1, sizeof(*word)); // calloc, чтобы если значение пустое, не передавать мусор.
    int wordSize = 1;
    int wordUsed = 0;

    size_t* maxWordLength = calloc(1, sizeof(*maxWordLength));
    int maxWordLengthSize = 1;

    int columnCount = 0;

    while (!feof(csv)) {
        char currentSymbol = (char)fgetc(csv);

        switch (currentSymbol) {
        case ',':
            if (rowUsed == maxWordLengthSize) {
                ++maxWordLengthSize;
                size_t* tempPtr = realloc(maxWordLength, maxWordLengthSize * sizeof(*tempPtr));
                assert(tempPtr != NULL && "Reallocation of memory is failed.");
                maxWordLength = tempPtr;
                maxWordLength[maxWordLengthSize - 1] = 0;
            }
            maxWordLength[rowUsed] = max(maxWordLength[rowUsed], strlen(word));

            if (rowUsed == rowSize) {
                rowSize *= 2;
                char** tempPtr = realloc(row, rowSize * sizeof(*tempPtr));
                assert(tempPtr != NULL && "Reallocation of memory is failed.");
                row = tempPtr;
            }
            row[rowUsed++] = word;
            word = calloc(1, sizeof(*word));
            wordSize = 1;
            wordUsed = 0;
            break;
        case '\n':
            columnCount = rowUsed + 1;

            if (rowUsed == maxWordLengthSize) {
                ++maxWordLengthSize;
                size_t* tempPtr = realloc(maxWordLength, maxWordLengthSize * sizeof(*tempPtr));
                assert(tempPtr != NULL && "Reallocation of memory is failed.");
                maxWordLength = tempPtr;
                maxWordLength[maxWordLengthSize - 1] = 0;
            }
            maxWordLength[rowUsed] = max(maxWordLength[rowUsed], strlen(word));

            if (rowUsed == rowSize) {
                rowSize *= 2;
                char** tempPtr = realloc(row, rowSize * sizeof(*tempPtr));
                assert(tempPtr != NULL && "Reallocation of memory is failed.");
                row = tempPtr;
            }
            row[rowUsed++] = word;
            word = calloc(1, sizeof(*word));
            wordSize = 1;
            wordUsed = 0;

            if (tableUsed == tableSize) {
                tableSize *= 2;
                char*** tempPtr = realloc(table, tableSize * sizeof(*tempPtr));
                assert(tempPtr != NULL && "Reallocation of memory is failed.");
                table = tempPtr;
            }
            table[tableUsed++] = row;
            row = malloc(sizeof(*row));
            rowSize = 1;
            rowUsed = 0;
            break;
        default:
            if (wordUsed == wordSize) {
                ++wordSize;
                char* tempPtr = realloc(word, wordSize * sizeof(*tempPtr));
                assert(tempPtr != NULL && "Reallocation of memory is failed.");
                word = tempPtr;
            }
            word[wordUsed++] = currentSymbol;
            break;
        }
    }

    free(row);
    free(word);

    table2txt(table, maxWordLength, tableUsed, columnCount);

    free(maxWordLength);
    for (int rowIndex = 0; rowIndex < tableUsed; ++rowIndex) {
        for (int colIndex = 0; colIndex < columnCount; ++colIndex) {
            free(table[rowIndex][colIndex]);
        }
        free(table[rowIndex]);
    }
    free(table);
}
