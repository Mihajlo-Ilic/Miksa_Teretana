#include "../util/cmd_parser.h"
#include <dirent.h>
#include <fcntl.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void printTabs(int depth)
{
  for (int i = 0; i < 4 * depth; i++)
  {
    printf("\t");
  }
}

int main(int argc, char **argv) {
  programName("dir");
  programVersion("1.0.0");
  programHelp("Lists contents of directories");
  argHelp("-r", "number", "Enter sub directories up to the passed depth");
  flagHelp("--r", "Enter sub directories");
  flagHelp("--h", "Prints in a nice readable format");
  flagHelp("--real", "Prints absolute paths");
  freeArgHelp(0, "path", "lists contents of a passed directory");

  parse(argc, argv);

  char *path = freeArgValue(0);

  if (path == NULL) {
    path = (char *)malloc(sizeof(char) * FILENAME_MAX);
    if (getcwd(path, FILENAME_MAX) == NULL) {
      perror("Failed to get current working directory");
      exit(EXIT_FAILURE);
    }
  }

  int maxDepth = 100;

  int humanReadable = hasFlag("h");
  int isRecursive = hasFlag("r") || hasArg("r");

  DIR *directory = opendir(path);
  if (directory == NULL) {
    perror("Failed to open directory");
    exit(EXIT_FAILURE);
  }

  char fullPath[FILENAME_MAX];

  DIR *stack[4096];
  int sSize = 1;
  stack[0] = directory;

  while (sSize != 0) {
    DIR *currDir = stack[sSize - 1];
    sSize--;
    struct dirent *dEntry;

    int dId = dirfd(currDir);
    fchdir(dId);

    while ((dEntry = readdir(currDir)) != NULL) {
      struct stat st;
      realpath(dEntry->d_name, fullPath);
      if (stat(fullPath, &st) != -1) {
        if (S_ISDIR(st.st_mode)) {
          if (strcmp(dEntry->d_name, ".") == 0 ||
              strcmp(dEntry->d_name, "..") == 0) {
            continue;
          }

          if (sSize < 4095) {
            directory = opendir(dEntry->d_name);
            if (directory == NULL) {
              perror("Failed to open directory");
              exit(EXIT_FAILURE);
            }
            stack[sSize] = directory;
            sSize++;
          }
        } else {
          printf("%s\n", dEntry->d_name);
        }
      }
      else
      {
        perror("greska");
      }
    }
    closedir(currDir);
  }

  cmdCleanUp();
  return EXIT_SUCCESS;
}
