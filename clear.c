#include "fileSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  FILE *fs;
  SuperBlock SB;
  INode IN;
  DataBlock DB;
  int iteration;
  char nullchar = '\0';

  /* Sprawdzenie czy system plików istnieje */
  fs = fopen("soiSystem", "r+b");
  if (!fs) {
    printf("Filesystem doesnt exist!\n");
    return -1;
  }

  /* Zainicjowanie zmiennych wartościami */

  SB.blockSize = DATA_BLOCK_SIZE;
  SB.dataBlockAmount = DATA_BLOCKS;
  SB.iNodeAmount = INODES;
  SB.iNodeOffset = DATA_BLOCK_SIZE;
  SB.dataBlockOffset = (INODES + 1) * DATA_BLOCK_SIZE;

  IN.isFull = 0;

  /* Zapisanie bloków do systemu plików */

  fwrite(&SB, sizeof(SuperBlock), 1, fs);
  fwrite(&nullchar, sizeof(char), DATA_BLOCK_SIZE - sizeof(SuperBlock), fs);
  for (iteration = 0; iteration < INODES + DATA_BLOCKS; iteration++) {
    fwrite(&IN.isFull, sizeof(char), 1, fs);
    fwrite(&nullchar, sizeof(char), DATA_BLOCK_SIZE - sizeof(char), fs);
  }
  fclose(fs);

  return 0;
}
