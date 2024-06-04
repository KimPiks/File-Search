#ifndef FILE_SEARCH_DIRECTORY_H
#define FILE_SEARCH_DIRECTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include "Options.h"

void openDirectory(DIR** dir, const char *searchPath);
void searchDirectory(const char *searchPath, struct Options options);

#endif
