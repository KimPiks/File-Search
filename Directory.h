#ifndef FILE_SEARCH_DIRECTORY_H
#define FILE_SEARCH_DIRECTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include "Options.h"

#define MAX_PATH_LENGTH 256
#define TIME_LENGTH 128
#define PERMISSIONS_LENGTH 12
#define TYPE_LENGTH 2

void openDirectory(DIR** dir, const char *searchPath);

void getStat(const char* path, struct stat* statbuf, struct dirent *pDirEnt);
void getCreationTime(struct stat statbuf, char* time_str);
void getPermissions(struct stat statbuf, char* permissions_str);
void getType(struct stat statbuf, char* type_str);

void showShortFileInfo(struct dirent *pDirEnt);

void showLongFileInfo(const char* path, struct dirent *pDirEnt);
void searchDirectory(const char *searchPath, struct Options options);

#endif
