#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "rm.h"
#include "utils.h"

void getRmPathName(char *pathname, size_t pathnameSize, char *options) {
  printf("%s$%s> rm %s", USERNAME, WORKING_DIR, options);
  getline(&pathname, &pathnameSize, stdin);
  trimInput(pathname);
}

void getRmOptions(char *options, size_t optionsSize) {
  printf("%s$%s> rm ", USERNAME, WORKING_DIR);
  getline(&options, &optionsSize, stdin);
  trimInput(options);
}

int checkIfDirectory(char *path) {
  struct stat pathStat;

  if (stat(path, &pathStat) != 0) {
    printf("Error occured while getting file stats");
    return 0;
  }

  return S_ISDIR(pathStat.st_mode);
}

int checkIfFile(char *path) {
  struct stat pathStat;

  if (stat(path, &pathStat) != 0) {
    printf("Error occured while getting file stats");
    return 0;
  }

  return S_ISREG(pathStat.st_mode);
}

int checkIfDirEmpty(char *path) {
  DIR *dir;
  struct dirent *entry;
  int isEmpty = 1;

  dir = opendir(path);
  if (dir == NULL) {
    printf("Failed to open Directory");
    return -1;
  }

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    isEmpty = 0;
    break;
  }

  closedir(dir);

  return isEmpty;
}

int removeDirRecursively(char *path) {}

void removeFileOrDir(char *path, int recursive, int force) {
  int isFile = checkIfFile(path);
  int isDir = checkIfDirectory(path);

  if (access(path, F_OK) != 0) {
    printf("%s: is not a valid path", path);
    return;
  }

  if (isDir) {
    if (checkIfDirEmpty(path) == 1) {
      if (rmdir(path) != 0) {
        if (!force) {
          printf("Error removing empty Directory");
        }
      }
    } else if (checkIfDirEmpty(path) == 0) {
      if (!recursive) {
        printf("You need do remove the not empty Directory recursive with -r");
        return;
      }
      if (removeDirRecursively(path) != 0) {
        if (!force) {
          printf("Couldnt remove Non empty Directory because of unknown error");
          return;
        }
      }
    }
  }
}

int checkIfOption(char *options, char option) {
  int optionsLength = sizeof(options) / sizeof(options[0]);
  int i = 0;

  for (i = 0; i < optionsLength; i++) {
    if (options[i] == '-') {
      if (options[i] == option) {
        return 1;
      }
    }
  }

  return 0;
}

void rm() {
  // First get any options
  // Then get the file/dir
  // check if it is a file or a dir
  // if it is a dir we need to check if it is empty
  // it it is not emtpy we need to check i the user has the -rf options
  // then remove the file/dir

  char *pathname;
  size_t pathnameSize = 2048;

  char *options;
  size_t optionsSize = 256;

  pathname = (char *)malloc(pathnameSize * sizeof(pathname));

  options = (char *)malloc(optionsSize * sizeof(options));

  getRmOptions(options, optionsSize);
  getRmPathName(pathname, pathnameSize, options);

  int recursive = checkIfOption(options, 'r');
  int force = checkIfOption(options, 'f');

  removeFileOrDir(pathname, recursive, force);

  free(pathname);
  free(options);
}
