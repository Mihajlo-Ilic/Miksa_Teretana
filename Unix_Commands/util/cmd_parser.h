#ifndef CMD_PARSER_H
#define CMD_PARSER_H

// Simple utility parser for command line arguments
// Flags are denoted by --
// Arguments by - followed by value
// If --help flag is provided the program exits and prints out instructions
// about using the program Example: ./a.out -arg value --flag1 --flag2 -arg2
// value2 ... etc

// Parses the command line arguments and stores them for further processing
int parse(int argc, char **argv);

// Returns 0 if a flag is present 1 otherwise
int hasFlag(const char *flag);

// Returns 0 if an argument is present 1 otherwise
int hasArg(const char *arg);

// Returns passed value for an argument
char *argValue(const char *arg);

// Returns value of i-th free argument without key
char *freeArgValue(int i);

// Sets a string to be displayed about program usage when --help is passed
void programHelp(const char *description);

// Information about flag in command section of --help
void flagHelp(const char *flag, const char *description);

// Sets a string to be displayed about a certain argument when --help is passed
void argHelp(const char *arg, const char *value, const char *description);

// Information about argument without a key in command section of --help
void freeArgHelp(int index, const char *value, const char *description);

// Sets a string to be displayed about program version when --verions is passed
void programVersion(const char *version);

// Sets a string for name of the program
void programName(const char *name);

// Sets a string to be displayed in usage section of --help
void usageExample(const char *cmd, const char *description);

// Frees up memory
void cmdCleanUp();

#endif // CMD_PARSER_H
