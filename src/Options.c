#include "Options.h"

void initOptions(int argc, char *argv[], struct Options *options)
{
  // Initialize all flags to false
  options->fullFileInformationFlag = false;
  options->recursiveSearchFlag = false;
  options->showHiddenFilesFlag = false;
  options->sortByTimeFlag = false;
  options->sortBySizeFlag = false;
  options->searchPath = "."; // Default search path is the current directory

  // Check each argument for the flags
  int i;
  for (i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-l") == 0)
    {
      options->fullFileInformationFlag = true;
    }
    else if (strcmp(argv[i], "-R") == 0)
    {
      options->recursiveSearchFlag = true;
    }
    else if (strcmp(argv[i], "-a") == 0)
    {
      options->showHiddenFilesFlag = true;
    }
    else if (strcmp(argv[i], "-t") == 0)
    {
      options->sortByTimeFlag = true;
    }
    else if (strcmp(argv[i], "-s") == 0)
    {
      options->sortBySizeFlag = true;
    }
    else
    {
      options->searchPath = argv[i];
    }
  }
}