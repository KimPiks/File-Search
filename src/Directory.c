#include "Directory.h"

// Open directory and handle errors
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

// Get file information
void getStat(const char* path, struct stat* statFile, struct dirent *pDirEnt)
{
  char fullPath[MAX_PATH_LENGTH+1];
  snprintf(fullPath, MAX_PATH_LENGTH+1, "%s/%s", path, pDirEnt->d_name);
  if (stat(fullPath, statFile) == -1) {
    return;
  }
}

// Get file creation time
void getCreationTime(struct stat stat, char* timeStr)
{
  strftime(timeStr, TIME_LENGTH, "%Y-%m-%d %H:%M:%S", localtime(&stat.st_ctime));
}

// Get file permissions
void getPermissions(struct stat stat, char* permissionsStr)
{
  // Get permissions from stat
  mode_t permissions = stat.st_mode & 0777;

  // Format permissions string to RWX-RWX-RWX format
  snprintf(permissionsStr, PERMISSIONS_LENGTH, "%c%c%c-%c%c%c-%c%c%c",
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

// Get file type
void getType(struct stat stat, char* type_str)
{
  if (S_ISREG(stat.st_mode)) {
    *type_str = 'f'; // regular file
  } else if (S_ISDIR(stat.st_mode)) {
    *type_str = 'd'; // directory
  } else if (S_ISCHR(stat.st_mode)) {
    *type_str = 'c'; // character device
  } else if (S_ISBLK(stat.st_mode)) {
    *type_str = 'b'; // block device
  } else if (S_ISFIFO(stat.st_mode)) {
    *type_str = 'p'; // FIFO (named pipe)
  } else if (S_ISLNK(stat.st_mode)) {
    *type_str = 'l'; // symbolic link
  } else if (S_ISSOCK(stat.st_mode)) {
    *type_str = 's'; // socket
  } else {
    *type_str = '?'; // unknown
  }
}

// Show file name (short info)
void showShortFileInfo(struct FileInfo file)
{
  printf("%s\n", file.name);
}

// Show file information (long info)
void showLongFileInfo(struct FileInfo file)
{
  printf("| %-21s | %-4c | %-13s | %-12s | %-10ld | %-30s |\n",
         file.creationTime, file.type, file.permissions, file.group, file.size, file.name);
}

// Compare files by name
int compareFilesByName(const void *a, const void *b) {
  const struct FileInfo entry1 = *(const struct FileInfo*)a;
  const struct FileInfo entry2 = *(const struct FileInfo*)b;
  return strcasecmp(entry1.name, entry2.name);
}

// Compare files by time
int compareFilesByTime(const void *a, const void *b) {
  const struct FileInfo entry1 = *(const struct FileInfo*)a;
  const struct FileInfo entry2 = *(const struct FileInfo*)b;
  return strcmp(entry1.creationTime, entry2.creationTime);
}

// Compare files by size
int compareFilesBySize(const void *a, const void *b) {
  const struct FileInfo entry1 = *(const struct FileInfo*)a;
  const struct FileInfo entry2 = *(const struct FileInfo*)b;

  if (entry1.size < entry2.size) {
    return -1;
  } else if (entry1.size > entry2.size) {
    return 1;
  } else {
    return compareFilesByName(a, b);
  }
}

void sortFiles(struct FileArray* files, struct Options options) {
  if (options.sortByTimeFlag) {
    qsort(files->files, files->size, sizeof(struct FileInfo), compareFilesByTime);
  } else if (options.sortBySizeFlag) {
    qsort(files->files, files->size, sizeof(struct FileInfo), compareFilesBySize);
  } else {
    qsort(files->files, files->size, sizeof(struct FileInfo), compareFilesByName);
  }
}

void setUpFileInfo(struct FileInfo* fileInfo, struct stat stat, struct dirent* dirEnt) {
  strcpy(fileInfo->name, dirEnt->d_name);
  getType(stat, &fileInfo->type);
  getPermissions(stat, fileInfo->permissions);
  getCreationTime(stat, fileInfo->creationTime);
  fileInfo->size = stat.st_size;
  strcpy(fileInfo->group, getgrgid(stat.st_gid)->gr_name);
}

// Get files from directory
void getFiles(DIR* pDIR, const char* searchPath, struct FileArray* files) {
  // Read entities from directory
  struct dirent *dirEnt;
  dirEnt = readdir(pDIR);
  while (dirEnt != NULL)
  {
    struct stat stat;
    getStat(searchPath, &stat, dirEnt);

    // Get file information
    struct FileInfo fileInfo;
    setUpFileInfo(&fileInfo, stat, dirEnt);

    // Add file to files array
    files->files[files->size] = fileInfo;
    files->size++;

    // Resize files array if needed
    if (files->size >= files->capacity) {
      files->capacity *= 2;
      files->files = realloc(files->files, files->capacity * sizeof(struct FileInfo));

      // Handle memory allocation error
      if (files->files == NULL) {
        perror("realloc");
        exit(1);
      }
    }

    // Read next entity
    dirEnt = readdir(pDIR);
  }
}

void addToRecursivePaths(struct RecursivePathsArray* recursivePaths, const char* searchPath, struct FileInfo file, struct Options options) {
  // Skip if not a directory
  if (file.type != 'd') return;
  // Skip "." and ".." directories
  if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0) return;
  // Skip hidden directories if needed
  if (options.showHiddenFilesFlag && file.name[2] == '.') return;

  char* recursivePath = malloc(MAX_PATH_LENGTH * sizeof(char));
  // Handle memory allocation error
  if (recursivePath == NULL) {
    perror("malloc");
    exit(1);
  }

  // Add directory to recursive paths array
  snprintf(recursivePath, MAX_PATH_LENGTH+1, "%s/%s", searchPath, file.name);
  recursivePaths->recursivePaths[recursivePaths->size] = recursivePath;
  recursivePaths->size++;

  // Resize recursive paths array if needed
  if (recursivePaths->size >= recursivePaths->capacity) {
    recursivePaths->capacity *= 2;
    recursivePaths->recursivePaths = realloc(recursivePaths->recursivePaths, recursivePaths->capacity * sizeof(char*));

    // Handle memory allocation error
    if (recursivePaths->recursivePaths == NULL) {
      perror("realloc");
      exit(1);
    }
  }
}

// Search directory and show files and directories
void searchDirectory(const char *searchPath, struct Options options)
{
  // Open directory
  DIR* pDIR = NULL;
  openDirectory(&pDIR, searchPath);

  // If full file information is required, show table header
  if (options.fullFileInformationFlag) {
    printf("| %-21s | %-4s | %-13s | %-12s | %-10s | %-30s |\n", "Created", "Type", "Permissions", "Group", "Size (B)", "File name");
    printf("|-----------------------|------|---------------|--------------|------------|--------------------------------|\n");
  }

  // Files array
  struct FileArray files;
  files.files = malloc(2 * sizeof(struct FileInfo));
  files.size = 0;
  files.capacity = 2;

  // Handle memory allocation error
  if (files.files == NULL) {
    perror("malloc");
    exit(1);
  }

  // Get files from directory
  getFiles(pDIR, searchPath, &files);

  // Sort files depending on options
  sortFiles(&files, options);

  // Recursive paths array (used when recursive search is enabled)
  struct RecursivePathsArray recursivePaths;
  recursivePaths.recursivePaths = malloc(2 * sizeof(char*));
  recursivePaths.size = 0;
  recursivePaths.capacity = 2;

  // Handle memory allocation error
  if (recursivePaths.recursivePaths == NULL) {
    perror("malloc");
    exit(1);
  }

  // Loop over files and directories
  int i;
  for (i = 0; i < files.size; i++)
  {
    // Skip hidden files if needed
    if (!options.showHiddenFilesFlag && files.files[i].name[0] == '.') {
      continue;
    }

    // Show file information depending on options
    if (options.fullFileInformationFlag) {
      showLongFileInfo(files.files[i]);
    } else {
      showShortFileInfo(files.files[i]);
    }

    // Add directory to recursive paths array if recursive search is enabled
    if (options.recursiveSearchFlag) {
      addToRecursivePaths(&recursivePaths, searchPath, files.files[i], options);
    }
  }

  // Close directory and free memory
  closedir(pDIR);
  free(files.files);

  // Recursive search
  if (options.recursiveSearchFlag) {
    for (i = 0; i < recursivePaths.size; i++) {
      printf("\n%s:\n", recursivePaths.recursivePaths[i]);
      searchDirectory(recursivePaths.recursivePaths[i], options);
      free(recursivePaths.recursivePaths[i]);
    }
  }

  free(recursivePaths.recursivePaths);
}