#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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
}
