#include "csv2txt.h"

int main(void)
{
    FILE* input = fopen("input.csv", "r");
    if (input == NULL) {
        printf("Please, run program from folder with input file.\n");
        return 1;
    }
    csv2txt(input);
    fclose(input);

    return 0;
}
