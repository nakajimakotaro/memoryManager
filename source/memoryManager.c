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
    MemoryBlockHeader tempMemoryHeader;
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
    memcpy(&tempMemoryHeader, memoryPoolHeader.freeMemoryList, sizeof(MemoryBlockHeader));

    allocateMemoryHeader->size = size;
    allocateMemoryHeader->prevFreeBlock = NULL;
    allocateMemoryHeader->nextFreeBlock = NULL;
    allocateMemoryHeader->isUse = true;

    newFreeMemoryHeader = (MemoryBlockHeader*)((int8_t*)allocateMemoryHeader + sizeof(MemoryBlockHeader) + allocateMemoryHeader->size);
    newFreeMemoryHeader->prevFreeBlock = tempMemoryHeader.prevFreeBlock;
    newFreeMemoryHeader->nextFreeBlock = tempMemoryHeader.nextFreeBlock;
    newFreeMemoryHeader->size = tempMemoryHeader.size - (allocateMemoryHeader->size + sizeof(MemoryBlockHeader));
    newFreeMemoryHeader->isUse = false;

    if(newFreeMemoryHeader->prevFreeBlock){
        newFreeMemoryHeader->prevFreeBlock->nextFreeBlock = newFreeMemoryHeader;
    }
    if(newFreeMemoryHeader->nextFreeBlock){
        newFreeMemoryHeader->nextFreeBlock->prevFreeBlock = newFreeMemoryHeader;
    }

    memoryPoolHeader.freeMemoryList = newFreeMemoryHeader;
    return allocateMemoryHeader->body;
}

void* myFree(void* ptr){
    return NULL;
}