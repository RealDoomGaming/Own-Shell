#include "mv.h"
#include "utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *mvGetDest() {
  char *filenameDest = NULL;
  size_t filenameSize = 1024;

  // read input
  printf("%s$%s> mv dest: ", USERNAME, WORKING_DIR);
  getline(&filenameDest, &filenameSize, stdin);
  trimInput(filenameDest);

  if (!filenameDest) {
    printf("No Destination File Name Specified");
    return NULL;
  }

  return filenameDest;
}

char *mvGetSource() {
  char *filenameSource = NULL;
  size_t filenameSize = 1024;

  // read input
  printf("%s$%s> mv source: ", USERNAME, WORKING_DIR);
  getline(&filenameSource, &filenameSize, stdin);
  trimInput(filenameSource);

  if (!filenameSource) {
    printf("No Source File Name Specified");
    return NULL;
  }

  return filenameSource;
}

void mv() {

  char *fileSource = mvGetSource();
  char *fileDest = mvGetDest();

  printf("Source: %s\n", fileSource);
  printf("Destination: %s\n", fileDest);

  if (fileSource == NULL) {
    printf("Couldnt Properly Get Source File, Exiting...\n");
    return;
  } else if (fileDest == NULL) {
    printf("Couldnt Properly Get Detination File, Exiting...\n");
    return;
  }

  // renaming the file
  rename(fileSource, fileDest);

  // freeing the pointers
  free(fileDest);
  free(fileSource);
}
