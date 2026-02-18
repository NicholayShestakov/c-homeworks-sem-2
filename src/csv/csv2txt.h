#pragma once

#include <stdio.h>

// Takes csv file and creates txt file with name "output.txt" and pretty table from csv. Not frees csv file.
void csv2txt(FILE* csv, char* outputPath);
