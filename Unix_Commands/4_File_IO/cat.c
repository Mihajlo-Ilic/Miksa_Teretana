#include "../util/cmd_parser.h"
#include "stdlib.h"

int main(int argc, char** argv)
{
  parse(argc, argv);
  argHelp("n", "Display line numbers"); 

  

  return EXIT_SUCCESS;
}
