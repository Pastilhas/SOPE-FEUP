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
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
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