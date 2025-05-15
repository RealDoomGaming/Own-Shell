#include "help.h"
#include "utils.h"
#include <malloc.h>
#include <stdio.h>

char *readFromFile(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    printf("There was an error Getting the file\n");
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);

  char *buffer = (char *)malloc(fileSize + 1);
  if (!buffer) {
    printf("There was an error allocation the memory\n");
    fclose(file);
    return NULL;
  }

  size_t bytesRead = fread(buffer, 1, fileSize, file);
  buffer[bytesRead] = '\n';

  rewind(file);

  fclose(file);
  return buffer;
}

void help() {
  // only reads the things from the text Manual file
  // and thats basically it this func doesnt do anything else

  // reading from the file and returning the read value
  char *readValue = readFromFile("./text/Manuall.md");
  if (!readValue) {
    printf("Could not read the Manual file\n");
    return;
  }

  // outputting the read text
  printf("%s", readValue);
}
