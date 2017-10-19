#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memoryManager.h"

const size_t POOL_SIZE = 200;

typedef struct _MemoryBlockHeader{
    char* prevFreeBlock;
    char* nextFreeBlock;
    size_t size;
    int isUse;
} MemoryBlockHeader;
typedef struct _MemoryPoolHeader{
    char* startPoint;
    char* lastPoint;
    MemoryBlockHeader* freeMemoryList;
} MemoryPoolHeader;
MemoryPoolHeader memoryPoolHeader = {};

void initMemory(){
    memoryPoolHeader.startPoint = malloc(POOL_SIZE);
    memoryPoolHeader.lastPoint = memoryPoolHeader.startPoint + POOL_SIZE;
    memoryPoolHeader.freeMemoryList = (MemoryBlockHeader*)memoryPoolHeader.startPoint;
    memoryPoolHeader.freeMemoryList->prevFreeBlock = NULL;
    memoryPoolHeader.freeMemoryList->nextFreeBlock = NULL;
    memoryPoolHeader.freeMemoryList->size = POOL_SIZE - sizeof(MemoryBlockHeader);
    memoryPoolHeader.freeMemoryList->isUse = 0;
}

void* myAllocate(size_t size){
    MemoryBlockHeader *newFreeMemoryHeader, *allocateMemoryHeader;
    MemoryBlockHeader tempMemoryHeader;
    allocateMemoryHeader = memoryPoolHeader.freeMemoryList;
    do{
        if(allocateMemoryHeader->size >= sizeof(MemoryBlockHeader) + size){
            break;
        }
        allocateMemoryHeader = (MemoryBlockHeader*)allocateMemoryHeader->nextFreeBlock;
    }while(allocateMemoryHeader);
    if(allocateMemoryHeader == NULL){
        return NULL;
    }
    memcpy(&tempMemoryHeader, memoryPoolHeader.freeMemoryList, sizeof(MemoryBlockHeader));

    allocateMemoryHeader->size = size;
    allocateMemoryHeader->prevFreeBlock = NULL;
    allocateMemoryHeader->nextFreeBlock = NULL;
    allocateMemoryHeader->isUse = 1;

    newFreeMemoryHeader = (MemoryBlockHeader*)((char*)allocateMemoryHeader + sizeof(MemoryBlockHeader) + allocateMemoryHeader->size);
    newFreeMemoryHeader->prevFreeBlock = tempMemoryHeader.prevFreeBlock;
    newFreeMemoryHeader->nextFreeBlock = tempMemoryHeader.nextFreeBlock;
    newFreeMemoryHeader->size = tempMemoryHeader.size - (allocateMemoryHeader->size + sizeof(MemoryBlockHeader));
    newFreeMemoryHeader->isUse = 0;

    memoryPoolHeader.freeMemoryList = newFreeMemoryHeader;
}

void* myFree(){
}

void debugPoolView(){
    printf("poolStart: %d\n", memoryPoolHeader.startPoint);
    printf("poolLast:  %d\n", memoryPoolHeader.lastPoint);
    printf("freePoint: %d\n", memoryPoolHeader.freeMemoryList);
    printf("freeSize:  %d\n", memoryPoolHeader.freeMemoryList->size);
    printf("\n");
}
void main(){
    initMemory();
    debugPoolView();
    myAllocate(10);
    debugPoolView();
    myAllocate(95);
    debugPoolView();
}
