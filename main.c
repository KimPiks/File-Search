#include "Options.h"

int main(int argc, char *argv[])
{
  // Parse command line options
  struct Options options;
  initOptions(argc, argv, &options);

  return 0;
}