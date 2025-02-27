#include "echo.h"
#include "utils.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void echoFuncOptions(char *options, size_t optionsSize) {
  printf("%s$%s> echo", USERNAME, WORKING_DIR);
  getline(&options, &optionsSize, stdin);
  trimInput(options);
}

void echoFuncOutput(char *output, size_t outputSize) {
  getline(&output, &outputSize, stdin);
  trimInput(output);
}

void echoFunc() {
  char *options;
  size_t optionsSize = 256;

  char *output;
  size_t outputSize = 1024;

  options = (char *)malloc(optionsSize * sizeof(options));
  output = (char *)malloc(outputSize * sizeof(output));

  echoFuncOptions(options, optionsSize);

  int sizeOfOptions = strlen(options);

  if (sizeOfOptions > 0) {
    printf("%s$%s> echo %s ", USERNAME, WORKING_DIR, options);
  } else if (sizeOfOptions == 0) {
    printf("%s$%s> echo ", USERNAME, WORKING_DIR);
  }

  echoFuncOutput(output, outputSize);

  int i = 0;
  for (i = 0; i < sizeOfOptions; i++) {
    if (options[i] == '-') {
      if (options[i + 1] == 'n') {
        strcat(options, "\n");
      } else {
        printf("%c is not a valid option \n", options[i + 1]);
      }
    }
  }

  printf("%s\n", output);

  free(options);
  free(output);
}
