// Lab 7
// 11603735

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Structure for MemoryAccess data
typedef struct
{
  char char_readWrite;
  int mainmemoryAdress;
  int numTags;
  int mmBlkNum;
  int cmSetNum;
  int cmBlkNum[2];
  char hitormiss;
} MemoryAccess;

// Structure for CacheMemory
typedef struct
{
  int blkNum;
  int dirtyBit;
  int validBit;
  int Tag;
  int dataBlk;
  int order;
} CacheMemory;

// Group of funcitons
int logBase2(int);
void findCacheSize(int, int);
int fileInput(MemoryAccess**, char*);
void memoryArray(MemoryAccess*, int, int, int, int, int);
void CacheBlkArray(CacheMemory*, int);
void findCacheBlocks(CacheMemory*, int, int);
void findMemoryAccess(MemoryAccess*, int);
int findRepeats(MemoryAccess*, int);
int numHits(MemoryAccess*, int);
void setCacheMemory(MemoryAccess*, int, CacheMemory*, int, char);

// main function
int main(void)
{
  // intitialize values
  int numMMBlks;
  int sizeMainMemory;
  int sizeCacheMemory;
  int sizeBlocks;
  int addessLines;
  int offsetBits;
  int indexBits;
  int cacheMemoryBlks;
  int cacheMemorySets;
  int tagBits;
  int totalVDbits;
  int memoryRepeats;
  int memoryHits;
  int memoryAccessArr;
  char replacementPolicy;
  char SA;
  char cont = 'y';
  char inputFile[100];
  MemoryAccess *MemoryAccessArr = NULL;

  // while user input is y, continue loop
  while(cont == 'y')
  {
    // Prompts user for input
    printf("Enter the size of main memory in bytes: ");
    scanf("%d", &sizeMainMemory);
    printf("Enter the size of the cache in bytes: ");
    scanf("%d", &sizeCacheMemory);
    printf("Enter the cache block/line size: ");
    scanf("%d", &sizeBlocks);
    printf("\nEnter the degree of set-associativity (input n for an n-way"
           " set-associative mapping): ");
    scanf(" %c", &SA);
    printf("Enter the replacement policy(L = LRU, F = FIFO): ");
    scanf(" %c", &replacementPolicy);
    printf("Enter the name of the input file containing the list of memory"
           " references generated by the CPU: ");
    scanf("%s", inputFile);

    // Sets memory values
    numMMBlks = sizeMainMemory/sizeBlocks;
    addessLines = round(log2(sizeMainMemory))  ;
    offsetBits = round(log2(sizeBlocks));
    cacheMemoryBlks = (sizeCacheMemory / sizeBlocks);
    cacheMemorySets = (SA == 'n') ? 1: (cacheMemoryBlks / (SA - '0'));
    tagBits = round(log2(numMMBlks/cacheMemorySets));
    indexBits = addessLines - offsetBits - tagBits;
    totalVDbits = (tagBits + 2)*cacheMemoryBlks;
    CacheMemory cacheBlkArr[cacheMemoryBlks];
    CacheBlkArray(cacheBlkArr, cacheMemoryBlks);

    // outputs to monitor
    printf("\n\nSimulator Output:\n");
    printf("Total address lines reguired = %d\n", addessLines);
    printf("Number of bits for offset = %d\n", offsetBits);
    printf("Number of bits for index = %d\n", indexBits);
    printf("Number of bits for tag = %d\n", tagBits);
    findCacheSize(sizeCacheMemory, totalVDbits);

    memoryAccessArr = fileInput(&MemoryAccessArr, inputFile);
    if(MemoryAccessArr != NULL)
    {
      memoryArray(MemoryAccessArr, memoryAccessArr, addessLines, sizeBlocks, cacheMemorySets,
                        (cacheMemoryBlks / cacheMemorySets));
    }
    else
    {
      printf("Error creating memory access array\n");
    }

    setCacheMemory(MemoryAccessArr, memoryAccessArr, cacheBlkArr, cacheMemoryBlks, replacementPolicy);
    findMemoryAccess(MemoryAccessArr, memoryAccessArr);

    memoryRepeats = findRepeats(MemoryAccessArr, memoryAccessArr);
    memoryHits = numHits(MemoryAccessArr, memoryAccessArr);
    printf("\nHighest possible hit rate = ");
    printf("%d/%d = ", memoryRepeats, memoryAccessArr);
    printf("%.2f%%\n", ((float)memoryRepeats/memoryAccessArr)*100);
    printf("Actual hit rate = ");
    printf("%d/%d = ", memoryHits, memoryAccessArr);
    printf("%.2f%%\n\n", ((float)memoryHits/memoryAccessArr)*100);

    printf("Final \"status\" of cache:\n");
    findCacheBlocks(cacheBlkArr, cacheMemoryBlks, tagBits);

    printf("Coninue?(y = yes, n = no): ");
    scanf(" %c", &cont);

    free(MemoryAccessArr);
  }
  return 0;
}




// Function to find the total size of the cache
void findCacheSize(int cacheSizeBytes, int numNonDataBits)
{
  int numBytes = cacheSizeBytes + numNonDataBits / 8;
  int numBits = 0;
  if((numNonDataBits % 8) != 0)
  {
    numBits = numNonDataBits % 8;
    printf("Total cache size required = %d bytes and %d bits\n", numBytes, numBits);
  }
  else
  {
    printf("Total cache size required = %d bytes\n", numBytes);
  }
}

int logBase2(int number)
{
  int base2OfNum = log((double)number) / log((double)2);
  return base2OfNum;
}

// inputs a file
int fileInput(MemoryAccess** MemoryAccessArr, char * inputFile)
{
  FILE *fp;
  char lineBuff[20];
  char addressStr[18];
  char numStr[20];
  int numReadWrites = 0;
  fp = fopen(inputFile, "r");
  // if file is not found
  if (fp == NULL) {
    printf("File could not be open.\n");
    exit(1);
  }
  // if file is found
  if(fp != NULL)
  {
    fgets(lineBuff, sizeof(lineBuff), fp);
    strcpy(numStr, &lineBuff[0]);
    numReadWrites = lineBuff[0] - '0';
    MemoryAccess *tempAccessArr = malloc(sizeof(MemoryAccess) * numReadWrites);
    if(tempAccessArr == NULL)
    {
      printf("Error! Could not allocate memory.\n");
      return 0;
    }

    fgets(lineBuff, sizeof(lineBuff), fp);
    int i = 0;
    for(; i < numReadWrites; i++ )
    {
      // finds the number of read and writes in file
      fgets(lineBuff, sizeof(lineBuff), fp);
      tempAccessArr[i].char_readWrite = lineBuff[0];
      strcpy(addressStr, &lineBuff[2]);
      tempAccessArr[i].mainmemoryAdress = atoi(addressStr);
    }

    *MemoryAccessArr = tempAccessArr;

    fclose(fp);
  }

  return numReadWrites;
}

// Sets memory array based on file inputs
void memoryArray(MemoryAccess* accessArr, int arraySize, int mmAddressLines, int sizeBlocks, int numSets, int setAsso)
{
  int offsetPlusIndexBits = logBase2(sizeBlocks) + logBase2(numSets); /* mmAddressLines - number of tag bits */
  int tagLenBitSet = pow(2, (mmAddressLines - offsetPlusIndexBits)) - 1; /* sets width of tag bits to all 1's */
  int i = 0;
  for(; i < arraySize; i++)
  {
    accessArr[i].numTags = ((accessArr[i].mainmemoryAdress >> offsetPlusIndexBits) & tagLenBitSet);
    accessArr[i].mmBlkNum = accessArr[i].mainmemoryAdress / sizeBlocks;
    accessArr[i].cmSetNum = accessArr[i].mmBlkNum % numSets;
    accessArr[i].cmBlkNum[0] = accessArr[i].cmSetNum * setAsso;
    accessArr[i].cmBlkNum[1] = accessArr[i].cmBlkNum[0] + (setAsso-1);
    accessArr[i].hitormiss = 'm'; /* initial to miss */
  }
}

// Cache block is initialized to a empty state
void CacheBlkArray(CacheMemory* cacheArray, int arraySize)
{
  int i = 0;
  for(; i < arraySize; i++)
  {
    cacheArray[i].blkNum = i;
    cacheArray[i].dirtyBit = 0;
    cacheArray[i].validBit = 0;
    cacheArray[i].Tag = -1;
    cacheArray[i].dataBlk = -1;
    cacheArray[i].order = -1;
  }
}

// Prints the values of the cache memory to user
void findCacheBlocks(CacheMemory* cacheArray, int arraySize, int tagBits)
{
  int i = 0;
  int j = 0;
  unsigned int bitShift = 0;
  printf("Cache blk #\t");
  printf("dirty bit\t");
  printf("valid bit\t");
  printf("tag\t\t");
  printf("Data\t\n");
  for(i=0; i<80; i++)
  {
    printf("=");
  }
  printf("\n");
  for(i=0;i<arraySize;i++)
  {
    bitShift = pow(2, tagBits-1);
    printf("%d\t\t", cacheArray[i].blkNum);
    printf("%d\t\t", cacheArray[i].dirtyBit);
    printf("%d\t\t", cacheArray[i].validBit);
    for(j=0;j<tagBits;j++)
    {
      if(cacheArray[i].Tag != -1)
      {
        printf("%d", (cacheArray[i].Tag & bitShift) > 0);
      }
      else
      {
        printf("x");
      }
      bitShift = bitShift >> 1;
    }
    printf("\t\t");
    if(cacheArray[i].dataBlk != -1)
    {
      printf("mm blk # %d", cacheArray[i].dataBlk);
    }
    else
    {
      printf("xxx");
    }
    printf("\n");
  }

}

// Prints the data of memory array to the user
void findMemoryAccess(MemoryAccess* accessArr, int arraySize)
{
  int i;

  // Prints to user monitor
  printf("\nmain memory address:\t");
  printf("mm blk #\t");
  printf("cm set #\t");
  printf("cm blk #\t");
  printf("hit/miss\n");
  for(i=0; i<=80; i++)
  {
    printf("=");
  }
  printf("\n");
  for(i = 0; i < arraySize; i++)
  {
      printf("%c", accessArr[i].char_readWrite);
      printf(" %d\t\t\t", accessArr[i].mainmemoryAdress);
      printf("%d\t\t", accessArr[i].mmBlkNum);
      printf("%d\t\t", accessArr[i].cmSetNum);
      if(accessArr[i].cmBlkNum[0] == accessArr[i].cmBlkNum[1])
      {
        printf("%d\t\t", accessArr[i].cmBlkNum[0]);
      }
      else
      {
        printf("%d - %d\t\t", accessArr[i].cmBlkNum[0], accessArr[i].cmBlkNum[1]);
      }
      // prints hit or miss
      if(accessArr[i].hitormiss == 'h')
      {
        printf("  hit\n");
      }
      else
      {
        printf("  miss\n");
      }
  }
}

// find repeat accesses to main memory
int findRepeats(MemoryAccess* accessArr, int arraySize)
{
  int totalRepeats = 0;
  int sortedMemBlkCalls[arraySize];
  int i, j, temp;


  for(i = 0; i < arraySize; i++)
  {
    sortedMemBlkCalls[i] = accessArr[i].mmBlkNum;
  }

  // sorting
  for(i = 0; i < arraySize - 1; i++)
  {
    for(j = 0; j < (arraySize - i - 1); j++ )
    {
      if(sortedMemBlkCalls[j] > sortedMemBlkCalls[j+1])
      {
        temp = sortedMemBlkCalls[j+1];
        sortedMemBlkCalls[j+1] = sortedMemBlkCalls[j];
        sortedMemBlkCalls[j] = temp;
      }
    }
  }


  for(i = 0; i < arraySize - 1; i++)
  {
    if(sortedMemBlkCalls[i+1] == sortedMemBlkCalls[i])
    {
      totalRepeats++;
    }
  }

  return totalRepeats;
}

// Finds the total number of hits in the cache
int numHits(MemoryAccess* accessArr, int arraySize)
{
  int numHits = 0;
  int i;

  for(i = 0; i < arraySize; i++)
  {
    if(accessArr[i].hitormiss == 'h')
    {
      numHits++;
    }
  }

  return numHits;
}

// function to fill the cache array
void setCacheMemory(MemoryAccess* accessArr, int accarraySize, CacheMemory* cmBlkArr, int cacheArrayLen, char repPolicy)
{
  int i, cmBlkIndex, cmBlkEnd, fifoIndex, willReplace;
  int numAccess = 0;

  for(i = 0; i < accarraySize; i++)
  {
    cmBlkIndex = accessArr[i].cmBlkNum[0];
    cmBlkEnd = accessArr[i].cmBlkNum[1];
    willReplace = 1;
    fifoIndex = cmBlkIndex;

    // looks for hit or load if it is empty
    while(cmBlkIndex <= cmBlkEnd)
    {
      // if false, load to cache memory
      if(!cmBlkArr[cmBlkIndex].validBit)
      {
        cmBlkArr[cmBlkIndex].validBit = 1;
        cmBlkArr[cmBlkIndex].Tag = accessArr[i].numTags;
        cmBlkArr[cmBlkIndex].dataBlk = accessArr[i].mmBlkNum;
        if(accessArr[i].char_readWrite == 'W')
        {
          cmBlkArr[cmBlkIndex].dirtyBit = 1;
        }

        cmBlkArr[cmBlkIndex].order = numAccess;

        willReplace = 0;
        break;
      }
      else if(cmBlkArr[cmBlkIndex].Tag == accessArr[i].numTags)
      {
        if(accessArr[i].char_readWrite == 'W')
        {
          cmBlkArr[cmBlkIndex].dirtyBit = 1;
        }

        accessArr[i].hitormiss = 'h';

        if(repPolicy == 'L' || repPolicy == 'l')
        {
          cmBlkArr[cmBlkIndex].order = numAccess;
        }
        willReplace = 0;
        break;
      }
      else if(cmBlkArr[cmBlkIndex].order < cmBlkArr[fifoIndex].order)
      {
        fifoIndex = cmBlkIndex;
      }

      cmBlkIndex++;
    }

    if(willReplace)
    {
      cmBlkArr[fifoIndex].dirtyBit = 0;
      cmBlkArr[fifoIndex].validBit = 1;
      cmBlkArr[fifoIndex].Tag = accessArr[i].numTags;
      cmBlkArr[fifoIndex].dataBlk = accessArr[i].mmBlkNum;
      cmBlkArr[fifoIndex].order = numAccess;
      if(accessArr[i].char_readWrite == 'W')
      {
        cmBlkArr[fifoIndex].dirtyBit = 1;
      }
    }

    numAccess++;
  }
}
