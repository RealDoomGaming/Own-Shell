#include "utils.h"
#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trimInput(char *input) {
  size_t inputSize = strlen(input);
  if (inputSize > 0 && input[inputSize - 1] == '\n') {
    input[inputSize - 1] = '\0';
  }
}

long countLines(FILE *file) {
  long count = 0;
  char buffer[MAX_LINE_COUNT_FOR_FILE];

  while (fgets(buffer, MAX_LINE_COUNT_FOR_FILE, file)) {
    count++;
  }

  rewind(file);
  return count;
}

int getInInput(char *options, char option) {
  int i = 0;
  int optionsLength = strlen(options);
  for (i = 0; i < optionsLength; i++) {
    if (options[i] == '-') {
      if (options[i + 1] == option) {
        return 1;
      }
    }
  }

  return 0;
}

int directoryExists(const char *path) {
  DIR *dir = opendir(path);
  if (dir) {
    closedir(dir);
    return 1;
  } else {
    return 0;
  }
}
