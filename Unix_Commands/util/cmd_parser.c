#include "cmd_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct s_pair {
  char *name;
  char *value;
};

struct helpArgument {
  const char *key;
  const char *value;
  const char *description;
};

// Array of flags passed to program
char **flags = NULL;
int flagsNum = 0;
int flagsAllocated = 0;

int flagCompare(const void *l, const void *r) {
  return strcmp((char *)l, *(char **)r);
}

int flagCompareSrt(const void *l, const void *r) {
  return strcmp(*(char **)l, *(char **)r);
}

// Array of s_pairs and their values passed to program
struct s_pair *arguments = NULL;
int argumentsNum = 0;
int argumentsAllocated = 0;

int argCompare(const void *l, const void *r) {
  return strcmp((char *)l, (*(struct s_pair *)r).name);
}

int argCompareSrt(const void *l, const void *r) {
  return strcmp((*(struct s_pair *)l).name, (*(struct s_pair *)r).name);
}

// Array of names and descriptions used for s_pairs and flags for --help
struct helpArgument *helpArguments = NULL;
int helpArgsNum = 0;
int helpArgsAllocated = 0;

// Array of examples and descriptions
struct s_pair *examples = NULL;
int examplesNum = 0;
int examplesAllocated = 0;

// Program meta info
const char *pName;
const char *pVersion;
const char *pDescription;

void pushExample(const char *cmd, const char *description) {
  examples[examplesNum].name = cmd;
  examples[examplesNum++].value = description;
}

void pushHelpArg(const char *arg, const char *value, const char *description) {
  helpArguments[helpArgsNum].key = arg;
  helpArguments[helpArgsNum].description = description;
  helpArguments[helpArgsNum++].value = value;
}

int isFlag(char *str) {
  if (str && str[0] == '-' && str[1] == '-') {
    return 0;
  }

  return 1;
}

void pushFlag(char *str) { flags[flagsNum++] = str + 2; }

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
  if (str && str[0] == '-') {
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
    arguments = (struct s_pair *)malloc(sizeof(struct s_pair));
    argumentsAllocated = 1;
  } else if (argumentsNum == argumentsAllocated) {
    argumentsAllocated <<= 1;
    arguments = (struct s_pair *)realloc(arguments, sizeof(struct s_pair) *
                                                        argumentsAllocated);
  }

  pushArg(arg, value);
}

void printSpaces(int n) {
  for (int l = 0; l < n; l++) {
    printf(" ");
  }
}

void printUnderScores(int n) {
  for (int l = 0; l < n; l++) {
    printf("-");
  }
}

void printHelp() {
  printf(" Help for %s\n\n", pName);

  printf("------DESCRIPTION------\n\n");
  printf(" %s\n\n", pDescription);

  if (helpArgsNum > 0) {
    printf(" The following table shows flags and arguments that can be passed "
           "to program\n");
    printf("  / in Value column indicates that arg is a flag and should be "
           "passed with -- prefix\n");
    printf("  $number in Arg column indicates that argument doesn't have a "
           "key.\n  Rather describes its index in free argument order\n\n");

    int maxArgLen = strlen(" Arg ");
    int maxDescriptionLen = strlen(" Description ");
    int maxValueLen = strlen(" Value ");

    for (int i = 0; i < helpArgsNum; i++) {
      int argLen = strlen(helpArguments[i].key) + 2;
      int descriptionLen = strlen(helpArguments[i].description) + 2;
      int valueLen = strlen(helpArguments[i].value) + 2;

      maxArgLen = argLen > maxArgLen ? argLen : maxArgLen;
      maxValueLen = valueLen > maxValueLen ? valueLen : maxValueLen;
      maxDescriptionLen = descriptionLen > maxDescriptionLen
                              ? descriptionLen
                              : maxDescriptionLen;
    }

    int freeSpace = maxArgLen - strlen(" Arg ");
    int leftPad = freeSpace / 2;
    int rightPad = freeSpace - leftPad;

    printf(" |");
    printSpaces(leftPad);
    printf(" Arg ");
    printSpaces(rightPad);

    freeSpace = maxValueLen - strlen(" Value ");
    leftPad = freeSpace / 2;
    rightPad = freeSpace - leftPad;

    printf(" |");
    printSpaces(leftPad);
    printf(" Value ");
    printSpaces(rightPad);

    freeSpace = maxDescriptionLen - strlen(" Description ");
    leftPad = freeSpace / 2;
    rightPad = freeSpace - leftPad;

    printf(" |");
    printSpaces(leftPad);
    printf(" Description ");
    printSpaces(rightPad);

    printf("|\n ");
    printUnderScores(maxArgLen + maxDescriptionLen + maxValueLen + 6);
    printf("\n");

    for (int i = 0; i < helpArgsNum; i++) {
      int argLen = strlen(helpArguments[i].key) + 2;
      int descriptionLen = strlen(helpArguments[i].description) + 2;
      int valueLen = strlen(helpArguments[i].value) + 2;

      int freeSpace = maxArgLen - argLen;
      int leftPad = freeSpace / 2;
      int rightPad = freeSpace - leftPad;

      printf(" |");
      printSpaces(leftPad);
      printf(" %s ", helpArguments[i].key);
      printSpaces(rightPad);

      freeSpace = maxValueLen - valueLen;
      leftPad = freeSpace / 2;
      rightPad = freeSpace - leftPad;

      printf(" |");
      printSpaces(leftPad);
      printf(" %s ", helpArguments[i].value);
      printSpaces(rightPad);

      freeSpace = maxDescriptionLen - descriptionLen;
      leftPad = freeSpace / 2;
      rightPad = freeSpace - leftPad;

      printf(" |");
      printSpaces(leftPad);
      printf(" %s ", helpArguments[i].description);
      printSpaces(rightPad);

      printf("|\n ");
      printUnderScores(maxArgLen + maxDescriptionLen + maxValueLen + 6);
      printf("\n");
    }

    printf("\n\n");
  }

  if (examplesNum > 0) {
    printf("------EXAMPLES------\n\n");

    for (int i = 0; i < examplesNum; i++) {
      printf(" %s\n  %s\n\n", examples[i].value, examples[i].name);
    }
  }
}

void printVersion() { printf("%s version: %s\n", pName, pVersion); }

int parse(int argc, char **argv) {
  for (int i = 1; i < argc; i++) {

    int isCurrentFlag = isFlag(argv[i]);
    int isCurrentArg = isArg(argv[i]);

    if (isCurrentFlag == 0) {
      addFlag(argv[i]);
    } else if (i == argc - 1 && isCurrentArg == 0) {
      fprintf(stderr, "Argument %s has no matching value\n", argv[i]);
      cmdCleanUp();
      exit(EXIT_FAILURE);
    } else if (isCurrentArg == 0 && isArg(argv[i + 1]) == 0) {
      fprintf(stderr, "Argument %s has no matching value\n", argv[i]);
      cmdCleanUp();
      exit(EXIT_FAILURE);
    } else if (isCurrentArg == 0) {
      addArg(argv[i], argv[i + 1]);
      i++;
    } else {
      addArg("-", argv[i]);
    }
  }

  qsort(flags, flagsNum, sizeof(char *), flagCompareSrt);
  qsort(arguments, argumentsNum, sizeof(struct s_pair), argCompareSrt);

  if (hasFlag("help") == 0) {
    printHelp();
    exit(EXIT_SUCCESS);
  }

  if (hasFlag("version") == 0) {
    printVersion();
    exit(EXIT_FAILURE);
  }

  return 0;
}

int hasFlag(const char *flag) {
  void *result = bsearch(flag, flags, flagsNum, sizeof(char *), flagCompare);

  if (result == NULL) {
    return 1;
  }

  return 0;
}

int hasArg(const char *arg) {
  void *result =
      bsearch(arg, arguments, argumentsNum, sizeof(struct s_pair), argCompare);

  if (result == NULL) {
    return 1;
  }

  return 0;
}

char *argValue(const char *arg) {
  void *result = bsearch(arg, arguments, argumentsNum,
                         sizeof(struct argument *), argCompare);
  return (char *)result;
}

void programHelp(const char *description) { pDescription = description; }

void programName(const char *name) { pName = name; }

void programVersion(const char *version) { pVersion = version; }

void argHelp(const char *arg, const char *value, const char *description) {
  if (helpArguments == NULL) {
    helpArguments = (struct helpArgument *)malloc(sizeof(struct helpArgument));
    helpArgsAllocated = 1;
  } else if (helpArgsNum == helpArgsAllocated) {
    helpArgsAllocated <<= 1;
    helpArguments = (struct helpArgument *)realloc(
        helpArguments, sizeof(struct helpArgument) * helpArgsAllocated);
  }

  pushHelpArg(arg, value, description);
}

void flagHelp(const char *flag, const char *description) {
  if (helpArguments == NULL) {
    helpArguments = (struct helpArgument *)malloc(sizeof(struct helpArgument));
    helpArgsAllocated = 1;
  } else if (helpArgsNum == helpArgsAllocated) {
    helpArgsAllocated <<= 1;
    helpArguments = (struct helpArgument *)realloc(
        helpArguments, sizeof(struct helpArgument) * helpArgsAllocated);
  }

  pushHelpArg(flag, "/", description);
}

void freeArgHelp(int index, const char *value, const char *description) {
  if (helpArguments == NULL) {
    helpArguments = (struct helpArgument *)malloc(sizeof(struct helpArgument));
    helpArgsAllocated = 1;
  } else if (helpArgsNum == helpArgsAllocated) {
    helpArgsAllocated <<= 1;
    helpArguments = (struct helpArgument *)realloc(
        helpArguments, sizeof(struct helpArgument) * helpArgsAllocated);
  }

  char *arg = (char *)malloc(5 * sizeof(char));
  sprintf(arg, "$%d", index);

  pushHelpArg(arg, value, description);
}

void usageExample(const char *cmd, const char *description) {
  if (examples == NULL) {
    examples = (struct s_pair *)malloc(sizeof(struct s_pair));
    examplesAllocated = 1;
  } else if (examplesAllocated == examplesNum) {
    examplesAllocated <<= 1;
    examples = (struct s_pair *)realloc(examples, sizeof(struct s_pair) *
                                                      examplesAllocated);
  }

  pushExample(cmd, description);
}

void cmdCleanUp() {
  if (arguments != NULL)
    free(arguments);
  if (flags != NULL)
    free(flags);
  if (helpArguments != NULL)
    free(helpArguments);
}
