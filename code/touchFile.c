#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "touchFile.h"
#include "utils.h"

void getFilePath(char *pathname, size_t pathnameSize) {
  printf("%s$%s> touch ", USERNAME, WORKING_DIR);
  getline(&pathname, &pathnameSize, stdin);
  trimInput(pathname);
}

void touchFile() {
  char *pathname;
  size_t pathnameSize = 1024;

  pathname = (char *)malloc(pathnameSize * sizeof(pathname));

  getFilePath(pathname, pathnameSize);

  FILE *file = fopen(pathname, "w");

  if (file == NULL) {
    printf("There was an error creating the %s file: %s", pathname,
           strerror(errno));
  }
}
