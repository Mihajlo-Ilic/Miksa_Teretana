#include "../util/cmd_parser.h"
#include <endian.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <time.h>
#include <grp.h>

int containsChar(char * str, char c)
{
  if (str == NULL)
  {
    return 1;
  }

  int i = 0;
  while(str[i])
  {
    if (str[i] == c)
    {
      return 0;
    }
    i++;
  }
  return 1;
}

int main(int argc, char **argv) {
  programName("stat");
  programVersion("1.0");
  programHelp("Print out statistics of a passed file/directory");

  argHelp("-filter", "string", "Only display data in the filter");
  flagHelp("a", "print access time");
  flagHelp("m", "print modification time");
  flagHelp("c", "print change time");
  flagHelp("b", "print birth time");
  flagHelp("t", "print file type");
  flagHelp("s", "print file size");
  flagHelp("u", "print user");
  flagHelp("g", "print group");

  freeArgHelp(0, "filepath", "Path to a file");

  usageExample("./stat.out a.c", "print out every statistic about a file");
  usageExample("./stat.out -filter amcb a.c", "print out only selected times of a.c file");

  parse(argc, argv);

  char *filePath = freeArgValue(0);
  if (filePath == NULL)
  {
    fprintf(stderr, "Path to a file is a required argument");
    exit(EXIT_FAILURE);
  }
  
  struct stat st;
  if (stat(filePath, &st) < 0)
  {
    perror("Failed to stat the file");
    exit(EXIT_FAILURE);
  }

  int noFilter = hasArg("filter");
  char *filter = argValue("filter");

  char time[60];
  char fullPath[PATH_MAX];
  realpath(filePath, fullPath);
  printf("File: %s", fullPath);

  if (S_ISBLK(st.st_mode))
  {
    printf(" special block file");
  }
  else if (S_ISCHR(st.st_mode))
  {
    printf(" special character file");
  }
  else if (S_ISDIR(st.st_mode))
  {
    printf(" directory");
  }
  else if (S_ISREG(st.st_mode))
  {
    printf(" regular file");
  }
  else if (S_ISLNK(st.st_mode))
  {
    printf(" symbolic link");
  }
  else 
  {
    printf("unknown");
  }

  printf("\n");

  if (noFilter == 1 || containsChar(filter, 'u'))
  {
    struct passwd *p = getpwuid(st.st_uid);
    printf("User: %s %d\n", p->pw_name, st.st_uid);
  }

  if (noFilter == 1 || containsChar(filter, 'g'))
  {
    struct group *g = getgrgid(st.st_gid);
    printf("Group: %s %d\n", g->gr_name, st.st_gid);
  }

  if (noFilter == 1 || containsChar(filter, 'a') == 0)
  {
    struct tm *a = gmtime(&(st.st_atim.tv_sec));
    strftime(time, 60, "%Y:%m:%d %H:%M:%S", a);
    printf("Access time: %s\n", time);
  }
  if (noFilter == 1 || containsChar(filter, 'm') == 0)
  {
    struct tm *a = gmtime(&(st.st_mtim.tv_sec));
    strftime(time, 60, "%Y:%m:%d %H:%M:%S", a);
    printf("Modify time: %s\n", time);
  }
  if (noFilter == 1 || containsChar(filter, 'c') == 0)
  {
    struct tm *a = gmtime(&(st.st_ctim.tv_sec));
    strftime(time, 60, "%Y:%m:%d %H:%M:%S", a);
    printf("Change time: %s\n", time);
  }


  if (noFilter == 1 || containsChar(filter, 'l') == 0)
  {
    printf("Links: %d\n", st.st_nlink);
  }
  
  if (noFilter == 1 || containsChar(filter, 'u') == 0)
  {
    printf("Blocks: %d\n", st.st_blocks);
  }

  if (noFilter == 1 || containsChar(filter, 's'))
  {
    printf("Size: %d\n", st.st_size);
  }

  if (noFilter == 1 || containsChar(filter, 'd'))
  {
    printf("Device: %d\n", st.st_dev);
  }

  if (noFilter == 1 || containsChar(filter, 'i'))
  {
    printf("iNode: %d\n", st.st_ino);
  }

  cmdCleanUp();

  return EXIT_SUCCESS;
}
