/* Operative Systems Project
 * by Joao Campos
 *
 * 18/03/2019
 */

/* Objetivos:
 * Percorrer sistema de ficheiros
 * Recolher/Criar informação sobre ficheiros
 * Apresentar a informação
 * Utilizador escolhe o diretório/ficheiro a ser lido
 *                    o ficheiro onde é colocada a informação
 *                    o local de geração de log files
 * Interrupção da execução a comando do utilizador
 */

/* foresinc
 * [-r]   analisar todos os ficheiros do diretório e subdiretorios
 * [-h]   calcular has dos ficheiros analisados
 * [-o]   guardar dados no ficheiro
 * [-v]   gerar log files
 */

#include "getters.h"

#define COMMAND_LOG 0
#define FILE_LOG 1

void dir_info(char *dirname);
void file_info(char *filename, char *d_name);
void getArgs(int argc, char **argv);
void getHash(char *type);
FILE *outputf(char *filename);
void rec_dir(char *path);
void log_write(int act, char *description);

static int arg[4];
static int hash[3];
static int argc_s;
static char **argv_s;
// static char *log_name;
static FILE *log;
static struct timespec time1;

int main(int argc, char **argv) {
  // forensic -r -h [type] -o [file] -v [file]

  // START OF TIME
  clock_gettime(CLOCK_MONOTONIC_RAW, &time1);

  if (argc < 2 || argc > 8) {
    if (argc < 2) {
      dprintf(STDERR_FILENO, "Not enough arguments.\n");
    } else {
      dprintf(STDERR_FILENO, "Too many arguments.\n");
    }
    exit(argc);
  }

  argc_s = argc;
  argv_s = argv;

  // ARGUMENTS
  getArgs(argc, argv);

  // HASH FLAGS
  if (arg[1])
    getHash(argv[arg[1]]);

  FILE *file;
  // OUTPUT FILE
  if (arg[2])
    file = outputf(argv[arg[2]]);

  // LOG FILE
  if (arg[3]) {
    char *log_name = getenv("LOGFILENAME");
    log = fopen(log_name, "w");
    if (log == NULL) {
      log = fopen("logs.txt", "w");
      if (log == NULL)
        exit(-1);
    }

    char command[100];

    strcat(command, argv[0]);
    for (int i = 1; i < argc; i++) {
      strcat(command, " ");
      strcat(command, argv[i]);
    }

    log_write(COMMAND_LOG, command);
  }

  // FORESINC
  char *file_name;
  if (isRoot(argv[argc - 1])) {
    file_name = (char *)malloc(strlen(argv[argc - 1]) + 1);
    strcpy(file_name, argv[argc - 1]);
  } else {
    file_name = (char *)malloc(strlen(argv[argc - 1]) + 3);
    strcpy(file_name, "./");
    strcat(file_name, argv[argc - 1]);
  }

  if (isDir(file_name)) {
    dir_info(file_name);
  } else {
    file_info(file_name, basename(file_name));
  }

  if (file)
    fclose(file);

  if (log)
    fclose(log);

  free(file_name);

  // WAIT FOR CHILD PROCESSES TO END
  pid_t wait_pid;
  do {
    wait_pid = wait(NULL); // RETURNS -1 ON ERROR AND SETS ECHILD IF NO UNWAITED CHILDREN
  } while (errno != ECHILD && wait_pid != -1);

  exit(0);
}

void dir_info(char *dirname) {
  DIR *dir_ptr = opendir(dirname);
  struct dirent *dirent;

  if (dir_ptr == NULL) {
    dprintf(STDERR_FILENO, "Cannot open.\n");
  } else {
    dirent = readdir(dir_ptr);
    while (dirent != NULL) {
      if (dirent->d_name[0] != '.') {
        char *filename =
            (char *)malloc(strlen(dirname) + strlen(dirent->d_name) + 2);

        // NEW ENTRY
        if (filename != NULL) {
          strcpy(filename, dirname);
          if (dirname[strlen(dirname) - 1] != '/')
            strcat(filename, "/");
          strcat(filename, dirent->d_name);

          // IF FILE
          if (!isDir(filename)) {
            // dprintf(STDERR_FILENO, "New file found: %s \n", filename);
            log_write(FILE_LOG, dirent->d_name);
            file_info(filename, dirent->d_name);

            // IF DIR
          } else if (arg[0]) {
            dprintf(STDERR_FILENO, "New dir found %s \n", filename);
            rec_dir(filename);
          }
          free(filename);
        } else {
          dprintf(STDERR_FILENO, "Failed to create file path.\n");
          exit(-1);
        }
      }

      dirent = readdir(dir_ptr);
    }
  }

  closedir(dir_ptr);
}

void file_info(char *filename, char *d_name) {
  struct stat info;
  get_stat(&info, filename, d_name);
  char final[200];

  // NAME
  strcpy(final, d_name);

  // TYPE
  char *type;
  type = getFileType(filename);
  strcat(final, ",");
  strcat(final, type);
  free(type);

  // SIZE
  int size = (int)info.st_size;
  char size_str[20];
  sprintf(size_str, "%d", size);
  strcat(final, ",");
  strcat(final, size_str);

  // ACCESS
  char *access = getAccess(info.st_mode);
  strcat(final, ",");
  strcat(final, access);
  free(access);

  // ACCESS DATE
  char *access_time = (char *)malloc(21 * sizeof(char));
  getDate(access_time, info.st_atime);
  strcat(final, ",");
  strcat(final, access_time);
  free(access_time);

  // MODIFICATION DATE
  char *mod_time = (char *)malloc(21 * sizeof(char));
  getDate(mod_time, info.st_mtime);
  strcat(final, ",");
  strcat(final, mod_time);
  free(mod_time);

  // HASH
  if (arg[1]) {
    if (hash[0]) { // MD5
      char *md5;
      md5 = getMD5(filename);
      strcat(final, ",");
      strcat(final, md5);
      free(md5);
    }

    if (hash[1]) { // SHA1
      char *sha1;
      sha1 = getSHA1(filename);
      strcat(final, ",");
      strcat(final, sha1);
      free(sha1);
    }

    if (hash[2]) { // SHA256
      char *sha256;
      sha256 = getSHA256(filename);
      strcat(final, ",");
      strcat(final, sha256);
      free(sha256);
    }
  }
  dprintf(STDOUT_FILENO, "%s\n", final);
}

void getArgs(int argc, char **argv) {
  // index indicates tag, value indicates index of aditional argument
  //    -r -h -o -v
  //     0  1  2  3

  for (int i = 0; i < argc; i++) {
    char *ar = argv[i];
    if (strcmp(ar, "-r") == 0) {
      arg[0] = 1;
    } else if (strcmp(ar, "-h") == 0) {
      arg[1] = i + 1;
      i++;
    } else if (strcmp(ar, "-o") == 0) {
      arg[2] = i + 1;
      i++;
    } else if (strcmp(ar, "-v") == 0) {
      arg[3] = 1;
    }
  }
}

void getHash(char *type) {
  size_t len = strlen(type);
  char tmp[10];
  char *tmp2;
  int size = 0;
  int i = 0;

  while ((unsigned)i <= len) {
    if (type[i] == ',' || type[i] == '\0') {
      size++;
      tmp2 = (char *)malloc(size * sizeof(char));
      memset(tmp2, '\0', size * sizeof(char));
      strcpy(tmp2, tmp);
      if (strcmp(tmp2, "md5") == 0)
        hash[0] = 1;
      else if (strcmp(tmp2, "sha1") == 0)
        hash[1] = 1;
      else if (strcmp(tmp2, "sha256") == 0)
        hash[2] = 1;
      free(tmp2);
      size = 0;
      memset(tmp, '\0', 10);
    } else {
      tmp[size] = type[i];
      size++;
    }
    i++;
  }
}

FILE *outputf(char *filename) {
  FILE *file = fopen(filename, "w+");
  if (file == NULL)
    exit(-3);
  dup2(fileno(file), STDOUT_FILENO);
  return file;
}

void rec_dir(char *path) {
  pid_t pid;

  pid = fork();

  if (pid > 0) {
    return;
  } else if (pid == 0) {
    dir_info(path);
    exit(0);
  } else {
    printf("failed fork\n");
    exit(-2);
  }
}

void log_write(int act, char *description) {
  char msg[300];

  // TIME OF LOG
  struct timespec time2;
  clock_gettime(CLOCK_MONOTONIC_RAW, &time2);
  double timediff =
      ((double)time2.tv_sec - (double)time1.tv_sec +
       ((double)time2.tv_nsec - (double)time1.tv_nsec) / 1000000000);

  // PID OF PROCESS
  int pid = (int)getpid();

  // ACTION
  char action[20];
  if (act == COMMAND_LOG)
    strcpy(action, "COMMAND");
  else if (act == FILE_LOG)
    strcpy(action, "ANALIZED");

  sprintf(msg, "%.2f - %d - %s  %s\n", timediff, pid, action, description);
  fprintf(log, "%s", msg);
  fflush(NULL);
}
