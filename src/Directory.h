#ifndef FILE_SEARCH_DIRECTORY_H
#define FILE_SEARCH_DIRECTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <grp.h>
#include "Options.h"

#define MAX_PATH_LENGTH 256
#define TIME_LENGTH 128
#define PERMISSIONS_LENGTH 12
#define GROUP_LENGTH 32

struct FileInfo
{
  char name[MAX_PATH_LENGTH];
  char type;
  char permissions[PERMISSIONS_LENGTH];
  char creationTime[TIME_LENGTH];
  char group[GROUP_LENGTH];
  long size;
};

struct FileArray {
  struct FileInfo* files;
  int size;
  int capacity;
};

struct RecursivePathsArray {
  char** recursivePaths;
  int size;
  int capacity;
};

void searchDirectory(const char *searchPath, struct Options options);

#endif
