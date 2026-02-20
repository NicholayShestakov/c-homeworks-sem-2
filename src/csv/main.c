#include "csv2txt.h"
#include <stdbool.h>
#include <string.h>

// Returns true if files is equal. Else returns false. Closes files.
bool filescmp(char* filePath1, char* filePath2)
{
    FILE* file1 = fopen(filePath1, "r");
    if (file1 == NULL) {
        return false;
    }
    FILE* file2 = fopen(filePath2, "r");
    if (file2 == NULL) {
        fclose(file1);
        return false;
    }

    while (!feof(file1) && !feof(file2)) {
        if (fgetc(file1) != fgetc(file2)) {
            fclose(file1);
            fclose(file2);
            return false;
        }
    }
    if (!feof(file1) || !feof(file2)) {
        fclose(file1);
        fclose(file2);
        return false;
    }

    fclose(file1);
    fclose(file2);
    return true;
}

// For tests, pass the "test" flag as input.
int test(void)
{
    // Empty test
    FILE* empty = fopen("test/testEmpty.csv", "r");
    if (empty == NULL) {
        printf("Please, run program from directory with 'test' folder.\n");
        return 1;
    }
    csv2txt(empty);
    fclose(empty);
    if (!filescmp("output.txt", "test/testEmptyOutput.txt")) {
        printf("Empty test is failed.\n");
        return 1;
    }

    // One column test
    FILE* oneColumn = fopen("test/testOneColumn.csv", "r");
    if (oneColumn == NULL) {
        printf("Please, run program from directory with 'test' folder.\n");
        return 1;
    }
    csv2txt(oneColumn);
    fclose(oneColumn);
    if (!filescmp("output.txt", "test/testOneColumnOutput.txt")) {
        printf("One column test is failed.\n");
        return 1;
    }

    // One row test
    FILE* oneRow = fopen("test/testOneRow.csv", "r");
    if (oneRow == NULL) {
        printf("Please, run program from directory with 'test' folder.\n");
        return 1;
    }
    csv2txt(oneRow);
    fclose(oneRow);
    if (!filescmp("output.txt", "test/testOneRowOutput.txt")) {
        printf("One row test is failed.\n");
        return 1;
    }

    // Unusual values
    FILE* unusualValues = fopen("test/testUnusualValues.csv", "r");
    if (unusualValues == NULL) {
        printf("Please, run program from directory with 'test' folder.\n");
        return 1;
    }
    csv2txt(unusualValues);
    fclose(unusualValues);
    if (!filescmp("output.txt", "test/testUnusualValuesOutput.txt")) {
        printf("Unusual values test is failed.\n");
        return 1;
    }

    remove("output.txt");

    printf("All tests successfully passed.\n");
    return 0;
}

int main(int argc, char* argv[])
{
    // For tests, pass the "test" flag as input.
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            if (strcmp("test", argv[i]) == 0) {
                return test();
            }
        }
    }

    FILE* input = fopen("input.csv", "r");
    if (input == NULL) {
        printf("Please, run program from directory with 'input.csv' file.\n");
        return 1;
    }
    csv2txt(input);

    return 0;
}
