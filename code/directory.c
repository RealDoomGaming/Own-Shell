#include "directory.h"
#include "utils.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void cdFuncDir(char *dir, size_t dirSize) {
  printf("%s$%s> cd ", USERNAME, WORKING_DIR);
  getline(&dir, &dirSize, stdin);
  trimInput(dir);
}

void readDirOptions(char *options, size_t optionsSize) {
  printf("%s$%s> ls ", USERNAME, WORKING_DIR);
  getline(&options, &optionsSize, stdin);
  trimInput(options);
}

int getPosOfLastSlash(char dir[1024]) {
  int posOfLastSlash = 0;

  int lenOfDir = strlen(dir);
  int i = 0;
  for (i = 0; i < lenOfDir; i++) {
    if (dir[i] == '/') {
      posOfLastSlash = i;
    }
  }

  return posOfLastSlash;
}

void goOneBack(char *newDir, int posOfLastSlash) {
  if (strlen(WORKING_DIR) == 1) {
    printf("Cannot go back even further\n");
    return;
  }

  if (posOfLastSlash == 0) {
    newDir[0] = '/';
    newDir[1] = '\0';
    return;
  }

  int lenOfDir = strlen(WORKING_DIR);
  int i = 0;
  while (i < lenOfDir && i < posOfLastSlash && i < 1023) {
    newDir[i] = WORKING_DIR[i];
    i++;
  }
  newDir[i] = '\0';
}

void goOneAhead(char *newDir, char *dirToAdd) {
  int lenOfCurrentDir = strlen(WORKING_DIR);
  int lenOfDirToAdd = strlen(dirToAdd);

  int i = 0;
  for (i = 0; i < lenOfCurrentDir; i++) {
    newDir[i] = WORKING_DIR[i];
  }

  newDir[lenOfCurrentDir] = '/';

  for (i = 0; i < lenOfDirToAdd; i++) {
    newDir[i + lenOfCurrentDir + 1] = dirToAdd[i];
  }
}

void cdFunc() {
  char *dir;
  size_t dirSize = 1024;

  dir = (char *)malloc(dirSize * sizeof(dirSize));

  cdFuncDir(dir, dirSize);

  char *newDir;
  size_t newDirSize = 1024;

  newDir = (char *)malloc(newDirSize * sizeof(newDir));

  int posOfLastSlash = getPosOfLastSlash(WORKING_DIR);

  if (strcmp(dir, "..") == 0) {
    goOneBack(newDir, posOfLastSlash);
  } else if (strcmp(dir, ".") == 0) {
    free(dir);
    free(newDir);
    return;
  } else {
    if (dir[0] == '/') {
      if (directoryExists(dir)) {
        strcpy(newDir, dir);
      } else {
        printf("Dir does not exist or is a file \n");
        free(dir);
        free(newDir);
        return;
      }
    } else if (dir[0] == '~') {
      strcpy(newDir, "/home/");
      strcat(newDir, USERNAME);

      printf("%s\n", newDir);

      memmove(dir, dir + 1, strlen(dir));

      printf("%s\n", dir);

      strcat(newDir, dir);

      if (!directoryExists(newDir)) {
        printf("Dir does not exist or is a file\n");
        free(dir);
        free(newDir);
        return;
      }
    } else {
      if (directoryExists(dir) == 1) {
        goOneAhead(newDir, dir);
      } else {
        printf("Dir does not exist or is a file\n");
        free(dir);
        free(newDir);
        return;
      }
    }
  }

  printf("New Dir: %s\n", newDir);
  setenv("PWD", newDir, 1);
  chdir(newDir);

  free(dir);
  free(newDir);
}

void readDir() {
  DIR *d;
  struct dirent *dir;
  d = opendir(WORKING_DIR);

  char *options;
  size_t optionsSize = 256;

  options = (char *)malloc(optionsSize * sizeof(options));

  readDirOptions(options, optionsSize);

  int optionLess = getInInput(options, 'l');
  int optionMore = getInInput(options, 'm');

  if (optionLess == 1) {
    optionMore = 0;
  }

  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if (optionLess == 1) {
        if (dir->d_name[0] != '.') {
          printf("%s\n", dir->d_name);
        }
      } else if (optionMore == 1) {
        printf("%s\n", dir->d_name);
      } else {
        if (strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".") != 0) {
          printf("%s\n", dir->d_name);
        }
      }
    }
    closedir(d);
  } else {
    printf("Dir does not exist or is a file\n");
    free(options);
    return;
  }

  free(options);
}
