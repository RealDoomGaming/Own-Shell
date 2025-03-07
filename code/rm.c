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
  printf("%s$%s> rm %s ", USERNAME, WORKING_DIR, options);
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
    printf("Error occured while getting file stats\n");
    return 0;
  }

  return S_ISDIR(pathStat.st_mode);
}

int checkIfFile(char *path) {
  struct stat pathStat;

  if (stat(path, &pathStat) != 0) {
    printf("Error occured while getting file stats\n");
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
    printf("Failed to open Directory\n");
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

int removeDirRecursively(char *path) {
  printf("Removing File recursively...\n");
  DIR *dir;
  struct dirent *entry;
  char fullPath[1024];
  struct stat fileStat;

  dir = opendir(path);
  if (dir == NULL) {
    printf("Error openin file to delete it recursively\n");
    return -1;
  }

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

    if (stat(fullPath, &fileStat) == -1) {
      printf("Error getting file Status\n");
      closedir(dir);
      return -1;
    }

    if (S_ISDIR(fileStat.st_mode)) {
      if (removeDirRecursively(fullPath) != 0) {
        closedir(dir);
        return -1;
      }
    } else {
      if (unlink(fullPath) != 0) {
        printf("Error removing file\n");
        closedir(dir);
        return -1;
      }
    }
  }

  closedir(dir);

  if (rmdir(path) != 0) {
    printf("Error removing directory\n");
    return -1;
  }

  return 0;
}

void removeFileOrDir(char *path, int recursive, int force) {
  int isFile = checkIfFile(path);
  int isDir = checkIfDirectory(path);

  if (access(path, F_OK) != 0) {
    printf("%s: is not a valid path\n", path);
    return;
  }

  if (isDir) {
    if (checkIfDirEmpty(path) == 1) {
      if (rmdir(path) != 0) {
        if (!force) {
          printf("Error removing Directory, try it with -f (force)\n");
        }
      }
    } else if (checkIfDirEmpty(path) == 0) {
      if (!recursive) {
        printf("Error removing Directory which is not empty, try it with -r "
               "(recursive)\n");
        return;
      }

      if (removeDirRecursively(path) != 0) {
        if (!force) {
          printf("Couldnt remove Directory whih isnt empty recursively, try it "
                 "with -f (force)\n");
          return;
        }
      }
    } else {
      printf("Path is not a file or directory, idk what you cooked");
      return;
    }
  } else if (isFile) {
    char *file = path;

    if (remove(file) == 0) {
      printf("Removed File sucesfully!\n");
    } else {
      printf("An error occured while removing the file\n");
    }
  } else {
    printf("Path is not a file or a dir\n");
  }
}

int checkIfOption(char *options, char option) {
  int optionsLength = strlen(options);
  int i = 0;

  for (i = 0; i < optionsLength; i++) {
    if (options[i] == '-') {
      if (options[i + 1] == option) {
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

  printf("%d\n", recursive);

  removeFileOrDir(pathname, recursive, force);

  free(pathname);
  free(options);
}
