#include "cp.h"
#include "utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

char *getSource() {
  char *filename = NULL;
  size_t filenameSize;

  // read input
  printf("%s$%s> cp Source: ", USERNAME, WORKING_DIR);
  getline(&filename, &filenameSize, stdin);
  trimInput(filename);

  // check if valid
  if (!filename) {
    printf("Could not get Source File");
    return NULL;
  }

  return filename;
}

char *getDest() {
  char *filename = NULL;
  size_t filenameSize;

  // read input
  printf("%s$%s> cp Dest: ", USERNAME, WORKING_DIR);
  getline(&filename, &filenameSize, stdin);
  trimInput(filename);

  // check if valid
  if (!filename) {
    printf("Could not get Destination File");
    return NULL;
  }

  return filename;
}

void cp() {
  char *filenameSource = getSource(), *filenameDest = getDest();

  if (filenameSource == NULL) {
    printf("Could not validate Source File, Exiting...");
    return;
  } else if (filenameDest == NULL) {
    printf("Could not validate Destination File, Exiting...");
    return;
  }

  // Whole cping file thingy:
  char buffer[1024];
  size_t bytesOfFile;
  FILE *sourceFile, *destFile;

  // get Source
  sourceFile = fopen(filenameSource, "r");

  if (sourceFile == NULL) {
    printf("Could not open Source File");
    fclose(sourceFile);
    return;
  }

  destFile = fopen(filenameDest, "w");

  if (destFile == NULL) {
    printf("Could Not Open Destination File");
    fclose(destFile);
    fclose(sourceFile);
    return;
  }

  while ((bytesOfFile = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0) {
    fwrite(buffer, 1, bytesOfFile, destFile);
  }

  free(filenameSource);
  free(filenameDest);
  fclose(sourceFile);
  fclose(destFile);

  return;
}
