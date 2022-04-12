#include "fileSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  FILE *fs;
  SuperBlock SB;
  INode IN;
  DataBlock DB;
  int offsetToData, nextoffsetToData, INodePosition;
  int INodeIndex, dataBlockIndex, chunks;
  char nullchar = '\0';
  char fileName[NAME_LENGTH];

  /* Sprawdzenie czy użytkownik podał nazwę pliku do usunięcia*/
  if (argc < 2) {
    printf("No file name was given\n");
    return -1;
  }

  /* Zapisanie nazwy pliku do zmiennej */
  strcpy(fileName, argv[1]);

  /* Sprawdzenie czy system plików istnieje */
  fs = fopen("soiSystem", "r+b");
  if (!fs) {
    printf("File system doesn't exist!\n");
    return -1;
  }

  /* Wczytanie informacji z superbloku */
  fread(&SB, sizeof(SuperBlock), 1, fs);
  fseek(fs, SB.iNodeOffset, SEEK_SET);

  for (INodeIndex = 0; INodeIndex < INODES; INodeIndex++) {
    /* Szukanie nazwy pliku w INodach */
    INodePosition = ftell(fs);
    fread(&IN, sizeof(INode), 1, fs);

    if (!strcmp(IN.name, fileName)) {
      /* Plik znaleziony więc go usuwamy */
      fseek(fs, INodePosition, SEEK_SET);

      offsetToData = IN.dataOffset;
      chunks = IN.size / DATA_BLOCK_SIZE;
      IN.isFull = 0;

      /* Zwolnienie INoda */
      fwrite(&IN.isFull, sizeof(char), 1, fs);
      fwrite(&nullchar, sizeof(char), DATA_BLOCK_SIZE - sizeof(char), fs);

      /* Zwalnianie bloków danych */
      for (dataBlockIndex = 0; dataBlockIndex < chunks; dataBlockIndex++) {
        /* Znalenienie offsetu do następnego bloku danych */
        fseek(fs, offsetToData, SEEK_SET);
        fread(&DB, sizeof(DataBlock), 1, fs);
        nextoffsetToData = DB.nextDataOffset;

        /* Zwolnienie bloku danych */
        fseek(fs, offsetToData, SEEK_SET);
        fwrite(&IN.isFull, sizeof(char), 1, fs);
        fwrite(&nullchar, sizeof(char), DATA_BLOCK_SIZE - sizeof(char), fs);
        offsetToData = nextoffsetToData;
      }

      /* Zwolnienie ostatniego bloku danych */
      fseek(fs, offsetToData, SEEK_SET);
      fwrite(&IN.isFull, sizeof(char), 1, fs);
      fwrite(&nullchar, sizeof(char), DATA_BLOCK_SIZE - sizeof(char), fs);

      fclose(fs);
      return 0;
    }
    /* Przejście do następnego INoda */
    fseek(fs, DATA_BLOCK_SIZE - sizeof(INode), SEEK_CUR);
  }

  /* Nie znaleziono danego pliu */
  fclose(fs);
  printf("No such file exists in file system\n");
  return -1;
}