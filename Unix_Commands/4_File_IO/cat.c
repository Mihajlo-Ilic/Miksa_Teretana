#include "../util/cmd_parser.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

int main(int argc, char **argv) {
  programName("cat");
  programVersion("1.0");
  programHelp("Prints out the contents of a file passed as an argument");

  flagHelp("--n", "Print out the line numbers along with the lines of a file");
  freeArgHelp(0, "filepath", "Path to a file you want to read");

  usageExample("./cat.o a.txt", "Print out contents of a.txt file");
  usageExample("./cat.o --n a.txt", "Print out contents and line numbers");

  parse(argc, argv);

  char *filePath = freeArgValue(0);

  if (filePath == NULL) {
    fprintf(stderr, "A path to a file needs to be passed");
    exit(EXIT_FAILURE);
  }

  int fd = open(filePath, O_RDONLY);
  if (fd == -1) {
    perror("Failed to open file");
    exit(EXIT_FAILURE);
  }

  char buffer[BUFFER_SIZE];

  if (hasFlag("n") == 1) {
    while (1) {
      int bytesRead = read(fd, buffer, BUFFER_SIZE);

      if (bytesRead < 0) {
        perror("Failed while reading from file");
        exit(EXIT_FAILURE);
      }

      if (bytesRead == 0) {
        break;
      }

      printf("%s", buffer);
    }
  } else {
  }

  close(fd);
  cmdCleanUp();

  return EXIT_SUCCESS;
}
