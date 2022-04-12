#include "fileSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  FILE *fs, *cpSrc;
  SuperBlock SB;
  INode IN;
  INode res;
  INode res3;
  DataBlock DB;
  DataBlock res2;
  DataBlock res4;
  char isInodeFree = 1;
  char isDataBlockFull = 1;
  char nullchar = '\0';
  int sourceFileSize = 0;
  int iteration = 0;
  int iteration2 = 0;
  int chunks;
  int lastDataBlock;
  char sourceFileContent[DATA_SIZE];
  char name[NAME_LENGTH];
  int currentINodePosition;
  int currentDataBlockPosition;
  int fileDataOffset;
  int offsetToNextDataBlock;

  if (argc < 2) {
    return -1;
  }

  /* Pobieranie superbloku z systemu */
  fs = fopen("soiSystem", "rb");
  if (!fs) {
    printf("No file system existing!\n");
    return -1;
  }
  fread(&SB, sizeof(SuperBlock), 1, fs);

  strcpy(name, argv[1]);
  /* Sprawdzenie czy dany plik znajduje sie już w systemie*/
  fseek(fs, SB.iNodeOffset, SEEK_SET);
  for (iteration = 0; iteration < INODES; iteration++) {
    fread(&IN, sizeof(INode), 1, fs);
    if (!strcmp(IN.name, name)) {
      printf("That file already exists in file system\n");
      return -1;
    }
    fseek(fs, DATA_BLOCK_SIZE - sizeof(INode), SEEK_CUR);
  }
  fclose(fs);

  /* Pobieranie informacji o pliku */
  cpSrc = fopen(argv[1], "rb");
  fseek(cpSrc, 0, SEEK_END);
  sourceFileSize = ftell(cpSrc);
  fseek(cpSrc, 0, SEEK_SET);
  chunks = sourceFileSize / DATA_SIZE;
  fclose(cpSrc);

  if (chunks >= DATA_BLOCKS) {
    return -1;
  }

  /* Zapis informacji do pierwszego wolnego inoda */
  fs = fopen("soiSystem", "r+b");
  fseek(fs, SB.iNodeOffset, SEEK_SET);
  while (isInodeFree == 1) {
    currentINodePosition = ftell(fs);
    fread(&IN, sizeof(INode), 1, fs);
    isInodeFree = IN.isFull;
    fseek(fs, SB.blockSize - sizeof(INode), SEEK_CUR);
  }
  IN.isFull = 1;
  strcpy(IN.name, argv[1]);
  IN.size = sourceFileSize;

  /* Przeskoczenie do pozycji pierwszego bloku danych */
  fseek(fs, SB.dataBlockOffset, SEEK_SET);

  cpSrc = fopen(argv[1], "r+b");

  isDataBlockFull = 1;
  while (isDataBlockFull == 1) {
    currentDataBlockPosition = ftell(fs);
    fread(&DB, sizeof(DataBlock), 1, fs);
    isDataBlockFull = DB.isFull;
  }

  for (iteration = 0; iteration <= chunks; iteration++) {
    if (iteration != chunks) {
      isDataBlockFull = 1;
      while (isDataBlockFull == 1) {
        offsetToNextDataBlock = ftell(fs);
        fread(&DB, sizeof(DataBlock), 1, fs);
        isDataBlockFull = DB.isFull;
      }
      DB.isFull = 1;
      DB.nextDataOffset = offsetToNextDataBlock;
      fread(&sourceFileContent, DATA_SIZE, 1, cpSrc);
      strcpy(DB.data, sourceFileContent);
      fseek(fs, currentDataBlockPosition, SEEK_SET);
      fwrite(&DB, sizeof(DataBlock), 1, fs);
    } else {
      if (offsetToNextDataBlock != 0) {
        currentDataBlockPosition = offsetToNextDataBlock;
      }
      DB.isFull = 1;
      lastDataBlock = sourceFileSize - (chunks * DATA_SIZE) + 1;
      fread(&sourceFileContent, lastDataBlock, 1, cpSrc);
      strcpy(DB.data, sourceFileContent);
      fseek(fs, currentDataBlockPosition, SEEK_SET);
      fwrite(&DB, lastDataBlock, 1, fs);
      fwrite(&nullchar, 1, DATA_SIZE - lastDataBlock, fs);
    }
    if (iteration == 0) {
      IN.dataOffset = currentDataBlockPosition;
      fseek(fs, currentINodePosition, SEEK_SET);
      fwrite(&IN, sizeof(INode), 1, fs);
    }
    currentDataBlockPosition = offsetToNextDataBlock;
    fseek(fs, currentDataBlockPosition + DATA_BLOCK_SIZE, SEEK_SET);
  }

  fclose(cpSrc);
  fclose(fs);

  return 0;
}
