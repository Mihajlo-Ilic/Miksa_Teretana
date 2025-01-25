#include "../util/cmd_parser.h"

#define _XOPEN_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <utime.h>
#include <time.h>

int main(int argc, char** argv)
{
  programName("touch");
  programVersion("1.0");
  programHelp("Program used to modify access time of file or create file");

  flagHelp("--a", "Modify only access time");
  flagHelp("--m", "Modify only modification time");
  argHelp("-time", "string", "Use this parses string instead of current time");
  freeArgHelp(0, "filepath", "Path to a file to modify or create");

  usageExample("./touch.o a.c", "Update file access times to current one or create file if it doesn't exist");
  usageExample("./touch.o -time 5.12.2024 15:26 a.c", "Use passed time instead of current one");

  parse(argc, argv);

  char* filePath = freeArgValue(0);
  if (filePath == NULL)
  {
    fprintf(stderr, "A path to a file needs to be passed");
    exit(EXIT_FAILURE);
  }

  int fd = open(filePath, O_RDONLY | O_CREAT);
  if (fd < 0)
  {
    perror("Failed to open file");
    exit(EXIT_FAILURE);
  }

  if (hasArg("time") == 1)
  {
    if (utime(filePath, NULL) < 0)
    {
      perror("Failed to set access and modification time");
      exit(EXIT_FAILURE);
    }
  }
  else {
    struct utimbuf timeBuffer;
    struct tm tm;

    if (NULL == strptime(argValue("time"), "%Y:%m:%d %H:%M:%S", &tm))
    {
      fprintf(stderr, "Bad format of date string");
      exit(EXIT_FAILURE);
    }
    
    time_t t = mktime(&tm);
    timeBuffer.actime = t;
    timeBuffer.modtime = t;

    if (utime(filePath, &timeBuffer) < 0)
    {
      perror("Failed to set access and modification time");
      exit(EXIT_FAILURE);
    }
  }

  close(fd);
  cmdCleanUp();

  return EXIT_SUCCESS;
}
