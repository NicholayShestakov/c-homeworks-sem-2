#pragma once

#include <stdio.h>

/*
Takes csv file.
At the same directory creates txt file with name "output.txt" and pretty table from csv.
Not closes csv file.
*/
void csv2txt(FILE* csv);
