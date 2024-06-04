#include "Options.h"
#include "Directory.h"

int main(int argc, char *argv[])
{
  // Parse command line options
  struct Options options;
  initOptions(argc, argv, &options);

  // Search the directory
  searchDirectory(options.searchPath, options);

  return 0;
}