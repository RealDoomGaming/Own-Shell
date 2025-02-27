#include <dirent.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_COUNT_FOR_FILE 1024
#define USERNAME getenv("USER")
#define WORKING_DIR getenv("PWD")

void trimInput(char *input) {
  size_t inputSize = strlen(input);
  if (inputSize > 0 && input[inputSize - 1] == '\n') {
    input[inputSize - 1] = '\0';
  }
}

void echoFuncOptions(char *options, size_t optionsSize) {
  printf("%s$%s> echo ", USERNAME, WORKING_DIR);
  getline(&options, &optionsSize, stdin);
  trimInput(options);
}

void echoFuncOutput(char *output, size_t outputSize) {
  getline(&output, &outputSize, stdin);
  trimInput(output);
}

void readFuncOptions(char *options, size_t optionsSize) {
  printf("%s$%s> read ", USERNAME, WORKING_DIR);
  getline(&options, &optionsSize, stdin);
  trimInput(options);
}

void readFuncFile(char *file, size_t fileSize) {
  getline(&file, &fileSize, stdin);
  trimInput(file);
}

void cdFuncDir(char *dir, size_t dirSize) {
  printf("%s$%s> cd ", USERNAME, WORKING_DIR);
  getline(&dir, &dirSize, stdin);
  trimInput(dir);
}

void readDirOptions(char *options, size_t optionsSize) {
  printf("%s$%s> ls ", USERNAME, WORKING_DIR);
  getline(&options, &optionsSize, stdin);
  trimInput(options);
}

void echoFunc() {
  // declaring the variables for the options and final output
  char *options;
  size_t optionsSize = 256;

  char *output;
  size_t outputSize = 1024;

  options = (char *)malloc(optionsSize * sizeof(options));
  output = (char *)malloc(outputSize * sizeof(output));

  // gets the echo options
  echoFuncOptions(options, optionsSize);

  // prints echo + options
  int sizeOfOptions = strlen(options);

  if (sizeOfOptions > 0) {
    printf("%s$%s> echo %s ", USERNAME, WORKING_DIR, options);
  } else if (sizeOfOptions == 0) {
    printf("%s$%s> echo ", USERNAME, WORKING_DIR);
  }

  // gets the echo output
  echoFuncOutput(output, outputSize);

  // processing the actual things of the echo function:
  // processes the options first
  int lenOfOptions = strlen(options);
  int i;

  for (i = 0; i < lenOfOptions; i++) {
    if (options[i] == '-') {
      if (options[i + 1] == 'n') {
        strcat(options, "\n");
      } else {
        printf("%c is not a valid option \n", options[i + 1]);
      }
    }
  }

  // then prints the user-output
  printf("%s\n", output);
}

// getting the file count of the file for things
long countLines(FILE *file) {
  long count = 0;
  char buffer[MAX_LINE_COUNT_FOR_FILE];

  // while it doesnt return NULL it does things;
  while (fgets(buffer, MAX_LINE_COUNT_FOR_FILE, file)) {
    // we count up so we have the max files
    count++;
  }

  // makes the pointer go to beginning of file
  rewind(file);
  return count;
}

// Function to read file from top to bottom:
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

// Function to read file from bottom to top
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
        return;
      } else if (options[i + 1] == 'b') {
        printf("%s$%s> read %s %s ", USERNAME, WORKING_DIR, options, file);
        scanf("%i", &fileCount);
        getchar();
        readFileFromTop(file, fileCount);
        return;
      } else {
        printf("%c is not a valid option \n", options[i + 1]);
        return;
      }
    }
  }

  readFileFromTop(file, countLines(fopen(file, "r")));

  free(options);
  free(file);
}

int getPosOfLastSlash(char dir[1024]) {
  int posOfLastSlash = 0;

  int lenOfDir = strlen(dir);
  int i = 0;
  for (i = 0; i < lenOfDir; i++) {
    if (dir[i] == '/') {
      posOfLastSlash = i;
    }
  }

  return posOfLastSlash;
}

int goOneBack(char *newDir, int posOfLastSlash) {
  if (strlen(WORKING_DIR) == 1) {
    printf("Cannot go back even further\n");
    return 1;
  }

  if (posOfLastSlash == 0) {
    newDir[0] = '/';
    newDir[1] = '\0';
    return 0;
  }

  int lenOfDir = strlen(WORKING_DIR);
  int i = 0;
  while (i < lenOfDir && i < posOfLastSlash && i < 1023) {
    newDir[i] = WORKING_DIR[i];
    i++;
  }
  newDir[i] = '\0';
  return 0;
}

void goOneAhead(char *newDir, char *dirToAdd) {
  int lenOfCurrentDir = strlen(WORKING_DIR);
  int lenOfDirToAdd = strlen(dirToAdd);

  int i = 0;
  for (i = 0; i < lenOfCurrentDir; i++) {
    newDir[i] = WORKING_DIR[i];
  }

  newDir[lenOfCurrentDir] = '/';

  for (i = 0; i < lenOfDirToAdd; i++) {
    newDir[i + lenOfCurrentDir + 1] = dirToAdd[i];
  }
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

void cdFunc() {
  char *dir;
  size_t dirSize = 1024;

  dir = (char *)malloc(dirSize * sizeof(dirSize));

  cdFuncDir(dir, dirSize);

  char *newDir;
  size_t newDirSize = 1024;

  newDir = (char *)malloc(newDirSize * sizeof(newDir));

  int posOfLastSlash = getPosOfLastSlash(WORKING_DIR);

  if (strcmp(dir, "..") == 0) {
    if (goOneBack(newDir, posOfLastSlash)) {
      goto cleanup;
    }
  } else if (strcmp(dir, ".") == 0) {
    return;
  } else {
    if (dir[0] == '/') {
      if (directoryExists(dir)) {
        strcpy(newDir, dir);
      } else {
        printf("Dir does not exist or is a file \n");
        return;
      }
    } else if (dir[0] == '~') {
      strcpy(newDir, "/home/");
      strcat(newDir, USERNAME);

      printf("%s\n", newDir);

      memmove(dir, dir + 1, strlen(dir));

      printf("%s\n", dir);

      strcat(newDir, dir);

      if (!directoryExists(newDir)) {
        printf("Dir does not exist or is a file\n");
        return;
      }
    } else {
      if (directoryExists(dir) == 1) {
        goOneAhead(newDir, dir);
      } else {
        printf("Dir does not exist or is a file\n");
        return;
      }
    }
  }

  printf("New Dir: %s\n", newDir);
  setenv("PWD", newDir, 1);
  chdir(newDir);

cleanup:
  free(dir);
  free(newDir);
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

void readDir() {
  DIR *d;
  struct dirent *dir;
  d = opendir(WORKING_DIR);

  char *options;
  size_t optionsSize = 256;

  options = (char *)malloc(optionsSize * sizeof(options));

  readDirOptions(options, optionsSize);

  int optionLess = getInInput(options, 'l');
  int optionMore = getInInput(options, 'm');

  if (optionLess == 1) {
    optionMore = 0;
  }

  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if (optionLess == 1) {
        if (dir->d_name[0] != '.') {
          printf("%s\n", dir->d_name);
        }
      } else if (optionMore == 1) {
        printf("%s\n", dir->d_name);
      } else {
        if (strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".") != 0) {
          printf("%s\n", dir->d_name);
        }
      }
    }
    closedir(d);
  } else {
    printf("Dir does not exist or is a file\n");
    return;
  }
}

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

  // printf("%s$%s> ", USERNAME, WORKING_DIR);
  // getline(&input, &inputSize, stdin);

  // while the input is not exit we will get inputs
  while (strcmp(input, "exit") != 0) {
    processInput(input); // calls the function to process the input ->
                         // differentiate between echo and cd

    // prints the default thin and scans a line
    printf("%s$%s> ", USERNAME, WORKING_DIR);
    getline(&input, &inputSize, stdin);

    trimInput(input);
  }

  free(input);

  return 0;
}
