#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "memoryManager.h"

const size_t POOL_SIZE = 200;

MemoryPoolHeader memoryPoolHeader = {};

void initMemory(){
    memoryPoolHeader.startPoint = malloc(POOL_SIZE);
    memoryPoolHeader.freeMemoryList = (MemoryBlockHeader*)memoryPoolHeader.startPoint;
    memoryPoolHeader.freeMemoryList->prevFreeBlock = NULL;
    memoryPoolHeader.freeMemoryList->nextFreeBlock = NULL;
    memoryPoolHeader.freeMemoryList->size = POOL_SIZE - sizeof(MemoryBlockHeader);
    memoryPoolHeader.freeMemoryList->isUse = false;
}

void* myAllocate(size_t size){
    MemoryBlockHeader *newFreeMemoryHeader, *allocateMemoryHeader;
    allocateMemoryHeader = memoryPoolHeader.freeMemoryList;
    while(allocateMemoryHeader){
        if(allocateMemoryHeader->size >= sizeof(MemoryBlockHeader) + size){
            break;
        }
        allocateMemoryHeader = (MemoryBlockHeader*)allocateMemoryHeader->nextFreeBlock;
    }
    if(allocateMemoryHeader == NULL){
        return NULL;
    }

    //BLockの切り出し
    allocateMemoryHeader->size = size;
    allocateMemoryHeader->prevFreeBlock = NULL;
    allocateMemoryHeader->nextFreeBlock = NULL;
    allocateMemoryHeader->isUse = true;
    memoryPoolHeader.freeMemoryList = memoryPoolHeader.freeMemoryList->nextFreeBlock;

    //余ったBLockの切り出し
    //TODO 要求サイズと同じBlocksizeの場合
    newFreeMemoryHeader = (MemoryBlockHeader*)((intptr_t)allocateMemoryHeader + sizeof(MemoryBlockHeader) + allocateMemoryHeader->size);
    newFreeMemoryHeader->size = memoryPoolHeader.freeMemoryList->size - (allocateMemoryHeader->size + sizeof(MemoryBlockHeader));
    myFree(newFreeMemoryHeader->body);

    return allocateMemoryHeader->body;
}

void* myFree(void* ptr){
    //TODO
    MemoryBlockHeader* freeBlockHeader = (MemoryBlockHeader*)((intptr_t)ptr - sizeof(MemoryBlockHeader));
    freeBlockHeader->isUse = false;
    freeBlockHeader->nextFreeBlock = memoryPoolHeader.freeMemoryList;
    freeBlockHeader->prevFreeBlock = NULL;
    memoryPoolHeader.freeMemoryList->prevFreeBlock = freeBlockHeader;
    memoryPoolHeader.freeMemoryList = freeBlockHeader;
}