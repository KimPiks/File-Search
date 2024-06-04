#include <time.h>
#include <sys/stat.h>
#include <grp.h>
#include "Directory.h"

void openDirectory(DIR** dir, const char *searchPath)
{
  *dir = opendir(searchPath);
  if (*dir == NULL)
  {
    fprintf(stderr, "%s %d: opendir() failed (%s)\n",
            __FILE__, __LINE__, strerror(errno));
    exit(-1);
  }
}

void showShortFileInfo(struct dirent *pDirEnt)
{
  printf("%s\n", pDirEnt->d_name);
}

void getStat(const char* path, struct stat* statbuf, struct dirent *pDirEnt)
{
  char fullPath[MAX_PATH_LENGTH+1];
  snprintf(fullPath, MAX_PATH_LENGTH+1, "%s/%s", path, pDirEnt->d_name);

  if (stat(fullPath, statbuf) == -1) {
    perror("stat");
    return;
  }
}

void getCreationTime(struct stat statbuf, char* time_str)
{
  strftime(time_str, TIME_LENGTH, "%Y-%m-%d %H:%M:%S", localtime(&statbuf.st_ctime));
}

void getPermissions(struct stat statbuf, char* permissions_str)
{
  mode_t permissions = statbuf.st_mode & 0777;

  snprintf(permissions_str, PERMISSIONS_LENGTH, "%c%c%c-%c%c%c-%c%c%c",
                               (permissions & S_IRUSR) ? 'r' : '-',
                               (permissions & S_IWUSR) ? 'w' : '-',
                               (permissions & S_IXUSR) ? 'x' : '-',
                               (permissions & S_IRGRP) ? 'r' : '-',
                               (permissions & S_IWGRP) ? 'w' : '-',
                               (permissions & S_IXGRP) ? 'x' : '-',
                               (permissions & S_IROTH) ? 'r' : '-',
                               (permissions & S_IWOTH) ? 'w' : '-',
                               (permissions & S_IXOTH) ? 'x' : '-');
}

void getType(struct stat statbuf, char* type_str)
{
  if (S_ISREG(statbuf.st_mode)) {
    *type_str = 'f';
  } else if (S_ISDIR(statbuf.st_mode)) {
    *type_str = 'd';
  } else if (S_ISCHR(statbuf.st_mode)) {
    *type_str = 'c';
  } else if (S_ISBLK(statbuf.st_mode)) {
    *type_str = 'b';
  } else if (S_ISFIFO(statbuf.st_mode)) {
    *type_str = 'p';
  } else if (S_ISLNK(statbuf.st_mode)) {
    *type_str = 'l';
  } else if (S_ISSOCK(statbuf.st_mode)) {
    *type_str = 's';
  } else {
    *type_str = '?';
  }
}

void showLongFileInfo(const char* path, struct dirent *pDirEnt)
{
  char time_str[TIME_LENGTH];
  char permissions_str[PERMISSIONS_LENGTH];
  char type_str[TYPE_LENGTH];
  struct stat statbuf;

  getStat(path, &statbuf, pDirEnt);
  getCreationTime(statbuf, time_str);
  getPermissions(statbuf, permissions_str);
  getType(statbuf, type_str);

  printf("| %-21s | %-4s | %-13s | %-12s | %-10ld | %-30s |\n",
         time_str, type_str, permissions_str, getgrgid(statbuf.st_gid)->gr_name, statbuf.st_size, pDirEnt->d_name);
}

void searchDirectory(const char *searchPath, struct Options options)
{
  DIR* pDIR = NULL;
  openDirectory(&pDIR, searchPath);

  if (options.fullFileInformationFlag) {
    printf("| %-21s | %-4s | %-13s | %-12s | %-10s | %-30s |\n", "Created", "Type", "Permissions", "Group", "Size (B)", "File name");
    printf("|-----------------------|------|---------------|--------------|------------|--------------------------------|\n");
  }

  struct dirent *pDirEnt;
  pDirEnt = readdir(pDIR);
  while (pDirEnt != NULL)
  {
    if (!options.showHiddenFilesFlag && pDirEnt->d_name[0] == '.') {
      pDirEnt = readdir(pDIR);
      continue;
    }

    if (options.fullFileInformationFlag) {
      showLongFileInfo(searchPath, pDirEnt);
    } else {
      showShortFileInfo(pDirEnt);
    }

    pDirEnt = readdir(pDIR);
  }
  closedir(pDIR);
}