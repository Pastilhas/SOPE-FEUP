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

void dir_info(char* dirname);
void file_info(char* filename, char* d_name);
void getArgs(int argc, char** argv);
void getHash(char* type);
FILE* outputf(char* filename);
void rec_dir(char* path);

static int arg[4];
static int hash[3];
static int argc_s;
static char** argv_s;

int main(int argc, char** argv) {
  // forensic -r -h [type] -o [file] -v [file]

  if (argc < 2 || argc > 8) {
    if (argc < 2) {
      write(STDOUT_FILENO, "Not enough arguments.\n", 22);
    } else {
      write(STDOUT_FILENO, "Too many arguments.\n", 20);
    }
    exit(argc);
  }

  argc_s = argc;
  argv_s = argv;

  getArgs(argc, argv);                // get args and change arg flags
  if (arg[1]) getHash(argv[arg[1]]);  // set hash flags
  FILE* file;
  if (arg[2]) {  // set outputfile
    file = outputf(argv[arg[2]]);
  }
  // char* log_name = getenv("LOGFILENAME");
  char* file_name;

  if (isRoot(argv[argc - 1])) {
    file_name = (char*)malloc(strlen(argv[argc - 1]) + 1);
    strcpy(file_name, argv[argc - 1]);
  } else {
    file_name = (char*)malloc(strlen(argv[argc - 1]) + 3);
    strcpy(file_name, "./");
    strcat(file_name, argv[argc - 1]);
  }

  if (isDir(file_name)) {
    dir_info(file_name);
  } else {
    file_info(file_name, basename(file_name));
  }
  if (arg[2]) fclose(file);

  exit(0);
}

void dir_info(char* dirname) {
  DIR* dir_ptr = opendir(dirname);  // pointer to directory
  struct dirent* dirent;            // pointer to entry

  if (dir_ptr == NULL) {  // Fail to open directory
    write(STDOUT_FILENO, "Cannot open.\n", 13);

  } else {
    dirent = readdir(dir_ptr);
    while (dirent != NULL) {
      if (dirent->d_name[0] != '.') {
        // Allocate memory for file path
        char* filename =
            (char*)malloc(strlen(dirname) + strlen(dirent->d_name) + 2);
        if (filename != NULL) {
          // Create path to file   dir/filename
          strcpy(filename, dirname);
          strcat(filename, "/");
          strcat(filename, dirent->d_name);
          if (!isDir(filename)) {
            file_info(filename, dirent->d_name);
          } else if (arg[0]) {  // RECURSIVE
            rec_dir(filename);
          }
          free(filename);
        } else {
          write(STDOUT_FILENO, "Failed to create file path.\n", 28);
          exit(-1);
        }
      }

      dirent = readdir(dir_ptr);
    }
  }

  closedir(dir_ptr);
}

void file_info(char* filename, char* d_name) {
  struct stat info;
  get_stat(&info, filename, d_name);

  // NAME
  printf("%s", d_name);

  // TYPE
  char* type;
  type = getFileType(filename);
  printf(",%s", type);
  free(type);

  // SIZE
  int size = (int)info.st_size;
  printf(",%d", size);

  // ACCESS
  char access[3];
  getAccess(access, info.st_mode);
  printf(",%s", access);

  // ACCESS DATE
  char* access_time = (char*)malloc(21 * sizeof(char));
  getDate(access_time, info.st_atime);
  printf(",%s", access_time);
  free(access_time);

  // MODIFICATION DATE
  char* mod_time = (char*)malloc(21 * sizeof(char));
  getDate(mod_time, info.st_mtime);
  printf(",%s", access_time);
  free(mod_time);

  if (hash[0]) {  // MD5
    char* md5 = "";
    getMD5(filename);
    printf(",%s", md5);
  }

  if (hash[1]) {  // SHA1
    char* sha1 = "";
    getSHA1(filename);
    printf(",%s", sha1);
  }

  if (hash[2]) {  // SHA256
    char* sha256 = "";
    getSHA256(filename);
    printf(",%s", sha256);
  }
  printf("\n");
}

void getArgs(int argc, char** argv) {
  // index indicates tag, value indicates index of aditional argument
  //    -r -h -o -v
  //     0  1  2  3

  for (int i = 0; i < argc; i++) {
    char* ar = argv[i];
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

void getHash(char* type) {
  size_t len = strlen(type);
  char tmp[10];
  char* tmp2;
  int size = 0;
  int i = 0;

  while ((unsigned)i <= len) {
    if (type[i] == ',' || type[i] == '\0') {
      tmp2 = (char*)malloc(size * sizeof(char));
      strcpy(tmp2, tmp);
      printf("%s\n", tmp2);
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
      size++;
      tmp[i] = type[i];
    }
    i++;
  }
}

FILE* outputf(char* filename) {
  FILE* file = fopen(filename, "w+");
  dup2(fileno(file), STDOUT_FILENO);
  return file;
}

void rec_dir(char* path) {
  pid_t pid;

  pid = fork();

  if (pid > 0) {
    return;
  } else if (pid == 0) {
    /* char* argv[argc_s];

    for(int i = 0; i < argc_s - 1; i++){
      argv[i] = argv_s[i];
    }
    argv[argc_s-1] = path; */

    argv_s[argc_s - 1] = path;
    execvp("./foresinc", argv_s);

    printf("failed exec\n");
    exit(-2);
  } else {
    printf("failed fork\n");
    exit(-2);
  }
}
