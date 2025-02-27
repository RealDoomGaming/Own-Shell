#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>

#define MAX_LINE_COUNT_FOR_FILE 1024
#define USERNAME getenv("USER")
#define WORKING_DIR getenv("PWD")

void trimInput(char *input);

long countLines(FILE *file);

int getInInput(char *options, char option);

int directoryExists(const char *path);

#endif // UTILS_H
