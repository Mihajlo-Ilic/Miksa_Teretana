#include "../util/cmd_parser.h"
#include <fcntl.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>

int main(int argc, char **argv) {
  programName("dir");
  programVersion("1.0.0");
  programHelp("Lists contents of directories");
  argHelp("-r", "number", "Enter sub directories up to the passed depth");
  flagHelp("--r", "Enter sub directories");
  flagHelp("--h", "Prints in a nice readable format");
  freeArgHelp(0, "path", "lists contents of a passed directory");

  parse(argc, argv);

  char* path = freeArgValue(0);

  if (path == NULL)
  {
    path = (char *) malloc(sizeof(char) * FILENAME_MAX);
    if (getcwd(path, FILENAME_MAX) == NULL)
    {
      perror("Failed to get current working directory");
      exit(EXIT_FAILURE);
    }
  }

  int maxDepth = 100;

  int humanReadable = hasFlag("h");
  int isRecursive = hasFlag("r") || hasArg("r");

  DIR* directory = opendir(path);
  if (directory == NULL)
  {
      perror("Failed to open directory");
      exit(EXIT_FAILURE);
  }

  DIR* stack[4096];
  int sSize = 1;
  stack[0] = directory;

  while(sSize != 0)
  {
    DIR* currDir = stack[sSize - 1];
    sSize--;

    struct dirent *dEntry;
    while((dEntry = readdir(currDir)) != NULL)
    {
        printf("%s\n", dEntry->d_name);
    }

  }

  cmdCleanUp();
  return EXIT_SUCCESS;
}
