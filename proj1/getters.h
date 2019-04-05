#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

void get_stat(struct stat *info, char *filename, char *d_name);
int isDir(char *line);
int isRoot(char *line);
char *getAccess(mode_t mode);
void getDate(char *str, time_t date);
char *getFileType(char *path);
char *getMD5(char *path);
char *getSHA1(char *path);
char *getSHA256(char *path);
