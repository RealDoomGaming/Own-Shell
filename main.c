#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_COUNT_FOR_FILE 1024
#define USERNAME getenv("USER")
#define WORKING_DIR getenv("PWD")

void echoFuncOptions(char options[256]) {
  printf("%s$%s> echo ", USERNAME, WORKING_DIR);
  scanf("%[^\n]s", options);
  getchar();
}

void echoFuncOutput(char output[1024]) {
  scanf("%[^\n]s", output);
  getchar();
}

void readFuncOptions(char options[256]) {
  printf("%s$%s> read ", USERNAME, WORKING_DIR);
  scanf("%[^\n]s", options);
  getchar();
}

void readFuncFile(char file[1024]) {
  scanf("%[^\n]s", file);
  getchar();
}

void cdFuncDir(char dir[1024]) {
  printf("%s$%s> cd ", USERNAME, WORKING_DIR);
  scanf("%[^\n]s", dir);
  getchar();
}

void echoFunc() {
  // declaring the variables for the options and final output
  char options[256];
  char output[1024];

  // gets the echo options
  echoFuncOptions(options);

  // prints echo + options
  int sizeOfOptions = strlen(options);

  if (sizeOfOptions > 0) {
    printf("%s$%s> echo %s ", USERNAME, WORKING_DIR, options);
  } else if (sizeOfOptions == 0) {
    printf("%s$%s> echo ", USERNAME, WORKING_DIR);
  }

  // gets the echo output
  echoFuncOutput(output);

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
  char options[256];
  char file[1024];

  // gets the read options
  readFuncOptions(options);

  // prints read + options
  int sizeOfOptions = strlen(options);

  if (sizeOfOptions > 0) {
    printf("%s$%s> read %s ", USERNAME, WORKING_DIR, options);
  } else if (sizeOfOptions == 0) {
    printf("%s$%s> read ", USERNAME, WORKING_DIR);
  }

  // gets the file to read
  readFuncFile(file);

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

void goOneBack(char newDir[1024], int posOfLastSlash) {
  if (strlen(WORKING_DIR) == 1 || posOfLastSlash == 0) {
    printf("Cannot go back even further\n");
    return;
  }

  int lenOfDir = strlen(WORKING_DIR);
  int i = 0;
  while (i < lenOfDir && i < posOfLastSlash && i < 1023) {
    newDir[i] = WORKING_DIR[i];
    i++;
  }
  newDir[i] = '\0';
}

void goOneAhead(char newDir[1024], char dirToAdd[1024]) {
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
  char dir[1024];

  cdFuncDir(dir);

  char newDir[1024] = {0};

  int posOfLastSlash = getPosOfLastSlash(WORKING_DIR);

  if (strcmp(dir, "..") == 0) {
    goOneBack(newDir, posOfLastSlash);
  } else if (strcmp(dir, ".") == 0) {
    return;
  } else {
    if (directoryExists(dir) == 1) {
      goOneAhead(newDir, dir);
    } else {
      return;
    }
  }

  printf("New Dir: %s\n", newDir);
  setenv("PWD", newDir, 1);
  chdir(newDir);
}

void processInput(char input[256]) {
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
    // printf("%s", readdir(WORKING_DIR));
  }
}

int main(int argc, char *argv[]) {
  // clearing the screen for better readability
  system("clear");

  // declaring the input variable
  char input[256] = "fakia";

  // while the input is not exit we will get inputs
  while (strcmp(input, "exit") != 0) {
    // fgets would be better but I cant get it to work
    // fgets(input, sizeof(input), stdin);
    processInput(input); // calls the function to process the input ->
                         // differentiate between echo and cd

    // scans a line and clears the line so nothing is there anymore
    printf("%s$%s> ", USERNAME, WORKING_DIR);
    scanf("%[^\n]s",
          input); // %[^\n]s will not ignore the space but also read it
    getchar();    // clears the line so scnaf doesnt read it
  }

  return 0;
}
