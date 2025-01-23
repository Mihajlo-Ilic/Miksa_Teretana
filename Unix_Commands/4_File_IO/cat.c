#include "../util/cmd_parser.h"
#include "stdlib.h"

int main(int argc, char** argv)
{
  programName("cat");
  programVersion("1.0");
  programHelp("Prints out the contents of a file passed as an argument");
  
  flagHelp("--n", "Print out the line numbers along with the lines of a file");

  usageExample("./cat.o a.txt", "Print out contents of a.txt file");
  usageExample("./cat.o --n a.txt", "Print out contents and line numbers");

  parse(argc, argv);
 

  cmdCleanUp();
  return EXIT_SUCCESS;
}
