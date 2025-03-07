#ifndef RM
#define RM

#include <stddef.h>

void getRMPathName(char *pathname, size_t pathnameSize, char *options);

void getRmOptions(char *options, size_t optionsSize);

int checkIfDirectory(char *path);

int checkIfFile(char *path);

int checkIfDirEmpty(char *path);

int removeDirRecursively(char *path);

void removeFileOrDir(char *path, int recusive, int force);

int checkIfOption(char *options, char option);

void rm();

#endif
