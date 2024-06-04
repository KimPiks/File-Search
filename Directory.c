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

void searchDirectory(const char *searchPath, struct Options options)
{
  DIR* pDIR = NULL;
  openDirectory(&pDIR, searchPath);

  struct dirent *pDirEnt;
  pDirEnt = readdir(pDIR);
  while (pDirEnt != NULL)
  {
    printf("%s\n", pDirEnt->d_name);
    pDirEnt = readdir(pDIR);
  }
  closedir(pDIR);
}