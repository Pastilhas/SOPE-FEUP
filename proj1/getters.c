#include "getters.h"

void get_stat(struct stat* info, char* filename, char* d_name) {
  if (stat(filename, info)) {
    perror(d_name);
  }
}

int isRoot(char* line) {
  if (line[0] == '/' || line[0] == '~')
    return 1;
  return 0;
}

int isDir(char* line) {
  struct stat info;
  get_stat(&info, line, line);

  return S_ISDIR(info.st_mode);
}

char* getAccess(mode_t mode) {
  char* access = (char*)malloc(4*sizeof(char));
  access[0] = (mode & S_IRUSR) ? 'r' : '-';
  access[1] = (mode & S_IWUSR) ? 'w' : '-';
  access[2] = (mode & S_IXUSR) ? 'x' : '-';
  return access;
}

void getDate(char* str, time_t date) {
  strftime(str, 20, "%Y-%m-%dT%H:%M:%S", localtime(&date));
}

char* getFileType(char* path) {
  int fd[2];
  int n;
  pid_t pid;

  if (pipe(fd) < 0)
    printf("failed pipe\n");

  pid = fork();

  if (pid > 0) {
    close(fd[WRITE]);
    char tmp[100];
    char tmp2[100];
    int i = 0;
    int length = 0;
    char* type;

    if ((n = read(fd[READ], tmp, 100)) < 0) {
      printf("fail to read %d\n", n);
      exit(-2);
    }

    while (i < 100) {
      if (tmp[i] == '\0' || tmp[i] == ',' || tmp[i] == '\n') {
        tmp2[length] = '\0';
        length++;
        break;
      } else {
        tmp2[length] = tmp[i];
        length++;
      }
      i++;
    }

    type = (char*)malloc(length * sizeof(char));
    memset(type, '\0', length*sizeof(char));
    strcpy(type, tmp2);
    return type;
  } else if (pid == 0) {
    close(fd[READ]);
    if (dup2(fd[WRITE], STDOUT_FILENO) == -1) {
      printf("failed dup2\n");
      exit(-2);
    }
    execlp("file", "file", "-b", path, NULL);
    printf("failed exec\n");
    exit(-2);
  } else {
    printf("failed fork\n");
    exit(-2);
  }
  exit(-2);
}

char* getMD5(char* path){
  int fd[2];
  int n;
  pid_t pid;

  if (pipe(fd) < 0)
    printf("failed pipe\n");

  pid = fork();

  if (pid > 0) {
    close(fd[WRITE]);
    char tmp[100];
    char tmp2[100];
    int i;
    int length = 0;
    char* type;

    if ((n = read(fd[READ], tmp, 100)) < 0) {
      printf("fail to read %d\n", n);
      exit(-2);
    }

    for (i = 0; i < 100; i++) {
      if (tmp[i] == '\0' || tmp[i] == ' ') {
        tmp2[length] = '\0';
        length++;
        break;
      } else {
        tmp2[length] = tmp[i];
        length++;
      }
    }

    type = (char*)malloc(length * sizeof(char));
    memset(type, '\0', length*sizeof(char));
    strcpy(type, tmp2);
    return type;
  } else if (pid == 0) {
    close(fd[READ]);
    if (dup2(fd[WRITE], STDOUT_FILENO) == -1) {
      printf("failed dup2\n");
      exit(-2);
    }
    execlp("md5sum", "md5sum", path, NULL);
    printf("failed exec\n");
    exit(-2);
  } else {
    printf("failed fork\n");
    exit(-2);
  }
  exit(-2);

}

char* getSHA1(char* path){
  int fd[2];
  int n;
  pid_t pid;

  if (pipe(fd) < 0)
    printf("failed pipe\n");

  pid = fork();

  if (pid > 0) {
    close(fd[WRITE]);
    char tmp[100];
    char tmp2[100];
    int i;
    int length = 0;
    char* type;

    if ((n = read(fd[READ], tmp, 100)) < 0) {
      printf("fail to read %d\n", n);
      exit(-2);
    }

    for (i = 0; i < 100; i++) {
      if (tmp[i] == '\0' || tmp[i] == ' ') {
        tmp2[length] = '\0';
        length++;
        break;
      } else {
        tmp2[length] = tmp[i];
        length++;
      }
    }

    type = (char*)malloc(length * sizeof(char));
    memset(type, '\0', length*sizeof(char));
    strcpy(type, tmp2);
    return type;
  } else if (pid == 0) {
    close(fd[READ]);
    if (dup2(fd[WRITE], STDOUT_FILENO) == -1) {
      printf("failed dup2\n");
      exit(-2);
    }
    execlp("sha1sum", "sha1sum", path, NULL);
    printf("failed exec\n");
    exit(-2);
  } else {
    printf("failed fork\n");
    exit(-2);
  }
  exit(-2);

}

char* getSHA256(char* path){
  int fd[2];
  int n;
  pid_t pid;

  if (pipe(fd) < 0)
    printf("failed pipe\n");

  pid = fork();

  if (pid > 0) {
    close(fd[WRITE]);
    char tmp[100];
    char tmp2[100];
    int i;
    int length = 0;
    char* type;

    if ((n = read(fd[READ], tmp, 100)) < 0) {
      printf("fail to read %d\n", n);
      exit(-2);
    }

    for (i = 0; i < 100; i++) {
      if (tmp[i] == '\0' || tmp[i] == ' ') {
        tmp2[length] = '\0';
        length++;
        break;
      } else {
        tmp2[length] = tmp[i];
        length++;
      }
    }

    type = (char*)malloc(length * sizeof(char));
    memset(type, '\0', length*sizeof(char));
    strcpy(type, tmp2);
    return type;
  } else if (pid == 0) {
    close(fd[READ]);
    if (dup2(fd[WRITE], STDOUT_FILENO) == -1) {
      printf("failed dup2\n");
      exit(-2);
    }
    execlp("sha256sum", "sha256sum", path, NULL);
    printf("failed exec\n");
    exit(-2);
  } else {
    printf("failed fork\n");
    exit(-2);
  }
  exit(-2);

}

char* itoa(int n){
  char const numbers[] = "0123456789";
  char tmp[100];
  char* tmp2;
  int size = n;
  int position = 0;

  while (size > 0)
  {
    position++;
    size = size/10;
  }

  tmp[position] = '\0';
  position--;
  tmp2 = (char*)malloc( position + 1 *sizeof(char));

  while (position >= 0) {
    tmp[position] = numbers[n%10];
    n /= 10;
    position--;
  }
  strcpy(tmp2, tmp);

  return tmp2;
}