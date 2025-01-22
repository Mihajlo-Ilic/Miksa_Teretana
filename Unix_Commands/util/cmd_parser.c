#include "cmd_parser.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct s_pair {
  char *name;
  char *value;
};

// Array of flags passed to program
char **flags = NULL;
int flagsNum = 0;
int flagsAllocated = 0;

int flagCompare(const void* l, const void* r)
{
  return strcmp((char*)l, (char*) r);
}

// Array of s_pairs and their values passed to program
struct s_pair *arguments = NULL;
int argumentsNum = 0;
int argumentsAllocated = 0;

int argCompare(const void* l, const void* r)
{
  return strcmp(((struct s_pair*)l)->name, ((struct s_pair*)r)->name);
}

// Array of names and descriptions used for s_pairs and flags for --help
struct s_pair *helpArguments = NULL;
int helpArgsNum = 0;
int helpArgsAllocated = 0;

// Program meta info
const char *pName;
const char *pVersion;
const char *pDescription;

int isFlag(char *str) {
  if (str && str[0] == '-' && str[1] == '-') {
    return 0;
  }

  return 1;
}

void pushFlag(char *str) { flags[flagsNum++] = str; }

void addFlag(char *str) {
  if (flags == NULL) {
    flags = (char **)malloc(sizeof(char *));
    flagsAllocated = 1;
  } else if (flagsNum == flagsAllocated) {
    flagsAllocated <<= 1;
    flags = (char **)realloc(flags, sizeof(char *) * flagsAllocated);
  }

  pushFlag(str);
}

int isArg(char *str) {
  if (str && str[0] == '-' && str[1] != '-') {
    return 0;
  }
  return 1;
}

void pushArg(char *arg, char *value) {
  arguments[argumentsNum].name = arg;
  arguments[argumentsNum++].value = value;
}

void addArg(char *arg, char *value) {
  if (arguments == NULL) {
    arguments = (struct s_pair *)malloc(sizeof(struct argument *));
    argumentsAllocated = 1;
  } else if (argumentsNum == argumentsAllocated) {
    argumentsAllocated <<= 1;
    arguments = (struct s_pair *)realloc(flags, sizeof(struct argument *) *
                                                    argumentsAllocated);
  }

  pushArg(arg, value);
}

void printHelp() {
  printf("Help for %s", pName);
  printf("---------");

  int len = strlen(pName);
  for (int i = 0; i < len; i++) {
    printf("-");
  }
  printf("\n");

  printf("%s\n", pDescription);
}

void printVersion() { printf("%s version: %s", pName, pVersion); }

int parse(int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    if (isFlag(argv[i]) == 0) {
      addFlag(argv[i]);
    } else if (isArg(argv[i]) == 0) {
      addArg(argv[i], argv[i + 1]);
      i++;
    }
  }

  if (hasFlag("help")) {
    printHelp();
    exit(EXIT_SUCCESS);
  }

  if (hasFlag("version")) {
    printVersion();
    exit(EXIT_FAILURE);
  }

  return 0;
}

int hasFlag(const char* flag)
{
  void* result = bsearch(flag, flags, flagsNum, sizeof(char*), flagCompare);

  if (result == NULL)
  {
    return 1;
  }

  return 0;
}


int hasArg(const char* arg)
{
  void* result = bsearch(arg, arguments, argumentsNum, sizeof(struct argument*), argCompare);

  if (result == NULL)
  {
    return 1;
  }

  return 0;
}

char* argValue(const char* arg)
{
  void* result = bsearch(arg, arguments, argumentsNum, sizeof(struct argument*), argCompare);
  return (char*)result;
}


void programHelp(const char* description)
{
  pDescription = description;
}


void programName(const char* name)
{
  pName = name;
}

void programVersion(const char* version)
{
  pVersion = version;
}
