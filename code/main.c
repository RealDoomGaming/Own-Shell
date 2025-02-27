#include "directory.h"
#include "echo.h"
#include "read.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void processInput(char *input) {
  // when you input echo it will go into the echo function since I want my shell
  // for you to first input the command, then the options and then the input
  if (strcmp(input, "echo") == 0) {
    echoFunc();
  } else if (strcmp(input, "read") == 0) {
    readFunc();
  } else if (strcmp(input, "clear") == 0) {
    system("clear");
  } else if (strcmp(input, "cd") == 0) {
    cdFunc();
  } else if (strcmp(input, "ls") == 0) {
    readDir();
  }
}

int main(int argc, char *argv[]) {
  // clearing the screen for better readability
  system("clear");

  // declaring the input variable and the size of the input variable;
  char *input;
  size_t inputSize = 256;

  input = (char *)malloc(inputSize * sizeof(input));

  // Initialize input to avoid undefined behavior
  input[0] = '\0';

  // while the input is not exit we will get inputs
  while (strcmp(input, "exit") != 0) {
    processInput(input); // calls the function to process the input ->
                         // differentiate between echo and cd

    // prints the default thing and scans a line
    printf("%s$%s> ", USERNAME, WORKING_DIR);
    getline(&input, &inputSize, stdin);

    trimInput(input);
  }

  free(input);

  return 0;
}
