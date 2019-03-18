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
  switch (argc) {
    case 2:     // forensic file
      break;
    case 3:     // forensic -r [file]
                // forensic -v [file]

      break;
    case 4:     // forensic -r -v [file]
                // forensic -h [type] [file]
                // forensic -o [file] [file]

      break;
    case 5:     // forensic -r -h [type] [file]
                // forensic -h [type] -v [file]
                // forensic -r -o [file] [file]
                // forensic -o [file] -h [file]

      break;
    case 6:     // forensic -r -h [type] -v [file]
                // forensic -r -o [file] -v [file]
                // forensic -h [type] -o [file] [file]

      break;
    case 7:     // forensic -r -h [type] -o [file] [file]
                // forensic -h [type] -o [file] -v [file]
                
      break;
    case 8:     // forensic -r -h [type] -o [file] -v [file]

      break;
    default:
      if (argc < 2) {
        write(STDOUT_FILENO, "Not enough arguments.\n", 22);
      } else {
        write(STDOUT_FILENO, "Too many arguments.\n", 20);
      }

      _exit(argc);
  }

  return 0;
}