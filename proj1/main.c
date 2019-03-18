/*
 *  Operative Systems Project
 *  by Joao Campos
 *     Diogo Sousa
 *     José Martins
 *
 *  18/03/2019
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

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

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
    _exit(argc);
  }
}