#include "fileSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  FILE *fs, *cpRes;
  SuperBlock SB;
  INode IN;
  DataBlock DB;
  int INodeIndex, DataBlockOffset, dataBlockIndex, chunks;
  char fileName[NAME_LENGTH];

  /* Sprawdzenie czy użytkownik podał nazwę pliku do wczytania*/
  if (argc < 2) {
    printf("No file name was given\n");
    return -1;
  }

  /* Zapis nazwy pliku do zmiennej */
  strcpy(fileName, argv[1]);

  /* Sprawdzenie czy system plików istanieje */
  fs = fopen("soiSystem", "r+b");
  if (!fs) {
    printf("No file system existing!\n");
    return -1;
  }

  /* Przesunięcie wskaźnika na adress pierwszego INoda */
  fread(&SB, sizeof(SuperBlock), 1, fs);
  fseek(fs, SB.iNodeOffset, SEEK_SET);

  /* Szukanie pliku o podanej nazwie w INodach */
  for (INodeIndex = 0; INodeIndex < INODES; INodeIndex++) {
    fread(&IN, sizeof(INode), 1, fs);
    if (!strcmp(IN.name, fileName)) {
      /* Plik został znaleziony więc zapisujemy go w minixie */
      chunks = IN.size / DATA_BLOCK_SIZE;
      fseek(fs, IN.dataOffset, SEEK_SET);
      cpRes = fopen(fileName, "a+b");

      /* Jeżeli plik ma rozmiar większy niż jeden blok to zapisujemy
      informacje blok po bloku */
      for (dataBlockIndex = 0; dataBlockIndex < chunks; dataBlockIndex++) {
        fread(&DB, DATA_BLOCK_SIZE, 1, fs);
        fwrite(&DB.data, DATA_SIZE, 1, cpRes);
        fseek(fs, DB.nextDataOffset, SEEK_SET);
      }

      /* Ostatni blok danych ma niestandardową wielkość dlatego
      należy zapisać go oddzielnie */
      fread(&DB, DATA_BLOCK_SIZE, 1, fs);
      fwrite(&DB.data, IN.size - (chunks * DATA_SIZE), 1, cpRes);

      /* Zamknięcie plików i zakończenie wczytywanie */
      fclose(cpRes);
      fclose(fs);
      return 0;
    }
    fseek(fs, DATA_BLOCK_SIZE - sizeof(INode), SEEK_CUR);
  }
  fclose(fs);

  /* Nie znaleziono pliku */
  printf("No such file exists in file system\n");
  return -1;
}