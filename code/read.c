#include "read.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void readFuncOptions(char *options, size_t optionsSize) {
  printf("%s$%s> read ", USERNAME, WORKING_DIR);
  getline(&options, &optionsSize, stdin);
  trimInput(options);
}

void readFuncFile(char *file, size_t fileSize) {
  getline(&file, &fileSize, stdin);
  trimInput(file);
}

void readFileFromTop(const char *filename, int numFile) {
  FILE *file = fopen(filename, "r");
  // checks if file can be found
  if (file == NULL) {
    printf("Error opening file: %s\n", filename);
    return;
  }

  char buffer[MAX_LINE_COUNT_FOR_FILE];
  int count = 0;

  // while we havent reached the file end and the count isnt bigger then the
  // user input (numFile)
  while (fgets(buffer, MAX_LINE_COUNT_FOR_FILE, file) &&
         (numFile == -1 || count < numFile)) {
    printf("%s", buffer);
    count++;
  }

  // closes the file stream
  fclose(file);
}

void readFileFromBottom(const char *filename, long numLines) {
  FILE *file = fopen(filename, "r");
  // checks if file can be found
  if (file == NULL) {
    printf("Error opening file: %s\n", filename);
    return;
  }

  // count total lines:
  long totalLines = countLines(file);

  // Calculate line where to start:
  long lineFromWhenToCount = totalLines - numLines;

  char buffer[MAX_LINE_COUNT_FOR_FILE];
  long currentLine = 0;

  while (fgets(buffer, MAX_LINE_COUNT_FOR_FILE, file)) {
    if (currentLine >= lineFromWhenToCount) {
      printf("%s", buffer);
    }
    currentLine++;
  }

  fclose(file);
}

void readFunc() {
  // declaring the variables for the options and the file to read
  char *options;
  size_t optionsSize = 256;

  char *file;
  size_t fileSize = 1024;

  options = (char *)malloc(optionsSize * sizeof(options));
  file = (char *)malloc(fileSize * sizeof(file));

  // gets the read options
  readFuncOptions(options, optionsSize);

  // prints read + options
  int sizeOfOptions = strlen(options);

  if (sizeOfOptions > 0) {
    printf("%s$%s> read %s ", USERNAME, WORKING_DIR, options);
  } else if (sizeOfOptions == 0) {
    printf("%s$%s> read ", USERNAME, WORKING_DIR);
  }

  // gets the file to read
  readFuncFile(file, fileSize);

  // processing the actuall things of the read file function:
  // getting the options and when getting the options also read and print the
  // file:
  int lenOfOptions = strlen(options);
  int i;
  int fileCount;

  for (i = 0; i < lenOfOptions; i++) {
    if (options[i] == '-') {
      if (options[i + 1] == 'e') {
        printf("%s$%s> read %s %s ", USERNAME, WORKING_DIR, options, file);
        scanf("%i", &fileCount);
        getchar();
        readFileFromBottom(file, fileCount);
        free(options);
        free(file);
        return;
      } else if (options[i + 1] == 'b') {
        printf("%s$%s> read %s %s ", USERNAME, WORKING_DIR, options, file);
        scanf("%i", &fileCount);
        getchar();
        readFileFromTop(file, fileCount);
        free(options);
        free(file);
        return;
      } else {
        printf("%c is not a valid option \n", options[i + 1]);
        free(options);
        free(file);
        return;
      }
    }
  }

  readFileFromTop(file, countLines(fopen(file, "r")));

  free(options);
  free(file);
}
