#include "printWorkingDir.h"
#include "utils.h"
#include <linux/limits.h>
#include <unistd.h>

void printWD() {
  char cwd[PATH_MAX];

  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("Current Working Directory Is: %s\n", cwd);
  } else {
    printf("Error Occured Trying To Get The Current Working Directory\n");
  }
}
