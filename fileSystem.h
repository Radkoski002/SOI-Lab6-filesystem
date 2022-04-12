#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#define FSSIZE 262144
#define INODES 15
#define DATA_BLOCKS 48
#define DATA_BLOCK_SIZE 4096
#define NAME_LENGTH 128
#define DATA_SIZE (DATA_BLOCK_SIZE - sizeof(int) - sizeof(char))

typedef struct SuperBlock {
  int blockSize;
  int iNodeAmount;
  int iNodeOffset;
  int dataBlockAmount;
  int dataBlockOffset;

} SuperBlock;

typedef struct INode {
  char isFull;
  int size;
  char name[NAME_LENGTH];
  int dataOffset;

} INode;

typedef struct DataBlock {
  char isFull;
  char data[DATA_SIZE];
  int nextDataOffset;

} DataBlock;

#endif
