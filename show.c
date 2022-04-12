#include "fileSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  FILE *fs;
  SuperBlock SB;
  INode IN;
  DataBlock DB;
  int dataBlocks[DATA_BLOCKS];
  int INodeIndex, dataBlockIndex;
  int index, currentPosition, chunks;

  /* Sprawdzenie czy system plików istnieje */
  fs = fopen("soiSystem", "r+b");
  if (!fs) {
    printf("File system doesn't exist!\n");
    return -1;
  }

  /* Wczytanie informacji z superbloku */
  fread(&SB, sizeof(SuperBlock), 1, fs);
  fseek(fs, SB.iNodeOffset, SEEK_SET);

  /* Sprawdzenie wartości w każdym INodzie */
  for (INodeIndex = 1; INodeIndex <= INODES; INodeIndex++) {
    fread(&IN, sizeof(INode), 1, fs);
    if (IN.isFull == 1) {
      /* Wyświetl informacje o INodzie jeżeli coś się w nim znajduje */
      printf("%d - %s, size: %d\n", INodeIndex, IN.name, IN.size);

      currentPosition = ftell(fs);
      chunks = IN.size / DATA_BLOCK_SIZE;

      /* Zapisz do mapy alokacji informacje o blokach danych */
      fseek(fs, IN.dataOffset, SEEK_SET);
      for (dataBlockIndex = 0; dataBlockIndex < chunks; dataBlockIndex++) {
        index = (ftell(fs) / DATA_BLOCK_SIZE) - INODES - 1;
        fread(&DB, sizeof(DataBlock), 1, fs);
        dataBlocks[index] = INodeIndex;
        fseek(fs, DB.nextDataOffset, SEEK_SET);
      }
      index = (ftell(fs) / DATA_BLOCK_SIZE) - INODES - 1;
      dataBlocks[index] = INodeIndex;
      fseek(fs, currentPosition, SEEK_SET);
    }

    /* Przejście do następnego INoda */
    fseek(fs, DATA_BLOCK_SIZE - sizeof(INode), SEEK_CUR);
  }

  /* Wyświetlenie mapy alokacji */
  for (INodeIndex = 0; INodeIndex < DATA_BLOCKS; INodeIndex++) {
    if (INodeIndex % 8 == 0) {
      printf("\n");
    }
    printf("%d ", dataBlocks[INodeIndex]);
  }
  printf("\n");
  return 0;
}