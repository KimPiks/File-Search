#ifndef FILE_SEARCH_OPTIONS_H
#define FILE_SEARCH_OPTIONS_H

#include <string.h>
#include <stdbool.h>

// Options structure with flags for each option
struct Options
{
  bool fullFileInformationFlag;
  bool recursiveSearchFlag;
  bool showHiddenFilesFlag;
  bool sortByTimeFlag;
  bool sortBySizeFlag;
};

// Initialize the options structure with the parameters passed in from the command line
void initOptions(int argc, char *argv[], struct Options *options);

#endif
