#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <stddef.h>

// Gets directory for cd command
void cdFuncDir(char *dir, size_t dirSize);

// Gets options for ls command
void readDirOptions(char *options, size_t optionsSize);

// Gets position of last slash in path
int getPosOfLastSlash(char dir[1024]);

// Go one directory back
void goOneBack(char *newDir, int posOfLastSlash);

// Go one directory ahead
void goOneAhead(char *newDir, char *dirToAdd);

// cd command implementation
void cdFunc();

// ls command implementation
void readDir();

#endif // DIRECTORY_H
