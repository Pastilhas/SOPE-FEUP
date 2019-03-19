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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void dir_info(char* dirname);
void file_info(char* filename, char* d_name);
void get_stat(struct stat* info, char* filename, char* d_name);

int main(int argc, char** argv) {
  // forensic file

  // forensic -r [file]
  // forensic -v [file]

  // forensic -r -v [file]
  // forensic -h [type] [file]
  // forensic -o [file] [file]

  // forensic -r -h [type] [file]
  // forensic -h [type] -v [file]
  // forensic -r -o [file] [file]
  // forensic -o [file] -h [file]

  // forensic -r -h [type] -v [file]
  // forensic -r -o [file] -v [file]
  // forensic -h [type] -o [file] [file]

  // forensic -r -h [type] -o [file] [file]
  // forensic -h [type] -o [file] -v [file]

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
  char* file_name = argv[argc - 1];

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

      dirent = readdir(dir_ptr);
    }
  }
  closedir(dir_ptr);
}

void file_info(char* filename, char* d_name) {
  struct stat info;
  get_stat(&info, filename, d_name);
}

void get_stat(struct stat* info, char* filename, char* d_name) {
  if (stat(d_name, info)) {
    perror(filename);
  }
}