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
    // If the argument does not start with a dash, it is the search path
    if (argv[i][0] != '-')
    {
      options->searchPath = argv[i];
      continue;
    }

    // Check flags
    if (strchr(argv[i], 'l') != NULL) options->fullFileInformationFlag = true;
    if (strchr(argv[i], 'R') != NULL) options->recursiveSearchFlag = true;
    if (strchr(argv[i], 'a') != NULL) options->showHiddenFilesFlag = true;
    if (strchr(argv[i], 't') != NULL) options->sortByTimeFlag = true;
    if (strchr(argv[i], 's') != NULL) options->sortBySizeFlag = true;
  }
}