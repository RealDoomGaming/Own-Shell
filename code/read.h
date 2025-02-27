#ifndef READ_H
#define READ_H

#include <stddef.h>

void readFuncOptions(char *options, size_t optionsSize);

void readFuncFile(char *file, size_t fileSize);

void readFileFromTop(const char *filename, int numFile);

void readFileFromBottom(const char *filename, long numLines);

void readFunc();

#endif // READ_H
