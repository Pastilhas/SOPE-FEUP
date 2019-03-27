/* Operative Systems Project
 * by Joao Campos
 *    Diogo Sousa
 *    José Martins
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

#include <dirent.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void dir_info(char* dirname);
void file_info(char* filename, char* d_name);
void get_stat(struct stat* info, char* filename, char* d_name);
int isDir(char* line);
int isRoot(char* line);

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

  char* log_name = getenv("LOGFILENAME");
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

  return 0;
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
          file_info(filename, dirent->d_name);
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

  char* name = filename;
  int size = (int)info.st_size;
  
  printf("%s,%d\n", d_name, size);
}

void get_stat(struct stat* info, char* filename, char* d_name) {
  if (stat(filename, info)) {
    perror(d_name);
  }
}

int isRoot(char* line) {
  if (line[0] == '/') return 1;
  return 0;
}

int isDir(char* line) {
  struct stat info;
  get_stat(&info, line, line);

  return S_ISDIR(info.st_mode);
}