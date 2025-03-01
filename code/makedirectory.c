#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "makedirectory.h"
#include "utils.h"

void getFileName(char *fileName, size_t fileNameSize) {
  printf("%s$%s> mkdir ", USERNAME, WORKING_DIR);
  getline(&fileName, &fileNameSize, stdin);
  trimInput(fileName);
}

void makeDir() {
  char *fileName;
  size_t fileNameSize = 2048;

  fileName = (char *)malloc(fileNameSize * sizeof(fileName));

  getFileName(fileName, fileNameSize);

  int sizeOfInputFileName = strlen(fileName);
  if (!mkdir(fileName, 0755) && sizeOfInputFileName != 0) {
    printf("%s was created succefully!!\n", fileName);
  } else {
    printf("There was an error creating the dir %s: %s\n", fileName,
           strerror(errno));
  }

  free(fileName);
}
