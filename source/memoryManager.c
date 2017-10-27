#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <lzma.h>
#include "memoryManager.h"

const size_t POOL_SIZE = 400;

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
    ptrdiff_t tempSize = memoryPoolHeader.freeMemoryList->size;


    //BLockの切り出し
    allocateMemoryHeader->size = size;
    allocateMemoryHeader->prevFreeBlock = NULL;
    allocateMemoryHeader->nextFreeBlock = NULL;
    allocateMemoryHeader->isUse = true;
    //TODO 最後の時何もしない
    ((MemoryBlockHeader*)((intptr_t)allocateMemoryHeader->body + allocateMemoryHeader->size))->prevBlockPoint = allocateMemoryHeader;

    //余ったBLockの切り出し
    //TODO 要求サイズと同じBlocksizeの場合
    newFreeMemoryHeader = (MemoryBlockHeader*)((intptr_t)allocateMemoryHeader->body + allocateMemoryHeader->size);
    newFreeMemoryHeader->size = tempSize - allocateMemoryHeader->size - sizeof(MemoryBlockHeader);
    //TODO 最後の時何もしない
    ((MemoryBlockHeader*)((intptr_t)newFreeMemoryHeader->body + newFreeMemoryHeader->size))->prevBlockPoint = newFreeMemoryHeader;
    newFreeMemoryHeader->isUse = false;
    newFreeMemoryHeader->nextFreeBlock = memoryPoolHeader.freeMemoryList->nextFreeBlock;
    newFreeMemoryHeader->prevFreeBlock = NULL;
    memoryPoolHeader.freeMemoryList->prevFreeBlock = newFreeMemoryHeader;
    memoryPoolHeader.freeMemoryList = newFreeMemoryHeader;



    return allocateMemoryHeader->body;
}

void myFree(void* ptr) {
    if (ptr == NULL) {
        return;
    }
    //メモリの開放　
    MemoryBlockHeader *freeBlockHeader = (MemoryBlockHeader *) ((intptr_t) ptr - sizeof(MemoryBlockHeader));
    freeBlockHeader->isUse = false;
    freeBlockHeader->nextFreeBlock = memoryPoolHeader.freeMemoryList;
    freeBlockHeader->prevFreeBlock = NULL;
    memoryPoolHeader.freeMemoryList->prevFreeBlock = freeBlockHeader;
    memoryPoolHeader.freeMemoryList = freeBlockHeader;

    //フリーメモリのマージ
    MemoryBlockHeader
            *prevMergeBlockHeader = freeBlockHeader->prevBlockPoint,
            *nextMergeBlockHeader = (MemoryBlockHeader *) ((intptr_t) freeBlockHeader->body + freeBlockHeader->size);
    nextMergeBlockHeader = nextMergeBlockHeader < (intptr_t)memoryPoolHeader.startPoint + POOL_SIZE ? nextMergeBlockHeader : NULL;

    //prevMergeBlockがfreeBlockを吸収する
    if (prevMergeBlockHeader && prevMergeBlockHeader->isUse == false) {
        memoryPoolHeader.freeMemoryList = freeBlockHeader->nextFreeBlock;
        freeBlockHeader->nextFreeBlock->prevFreeBlock = NULL;
        //prevMergeBlockHeaderのsizeをfreeBlockHeaderのラストまで伸ばす
        prevMergeBlockHeader->size = ((intptr_t) freeBlockHeader->body + freeBlockHeader->size) - (intptr_t) prevMergeBlockHeader->body;
        freeBlockHeader = prevMergeBlockHeader;
        if ((intptr_t) freeBlockHeader->body + freeBlockHeader->size + sizeof(MemoryBlockHeader) < (intptr_t)memoryPoolHeader.startPoint + POOL_SIZE) {
            ((MemoryBlockHeader *)((intptr_t) freeBlockHeader->body + freeBlockHeader->size))->prevBlockPoint = freeBlockHeader;
        }
    }
    if (nextMergeBlockHeader && nextMergeBlockHeader->isUse == false) {
        nextMergeBlockHeader->prevFreeBlock->nextFreeBlock = nextMergeBlockHeader->nextFreeBlock;
        if(nextMergeBlockHeader->nextFreeBlock){
            nextMergeBlockHeader->nextFreeBlock->prevFreeBlock = NULL;
        }
        //FreeBlockのサイズをnextMergeBlockHeaderのラストまで伸ばす
        freeBlockHeader->size = ((intptr_t) nextMergeBlockHeader->body + nextMergeBlockHeader->size) - (intptr_t) freeBlockHeader->body;
        if ((intptr_t) freeBlockHeader->body + freeBlockHeader->size + sizeof(MemoryBlockHeader) < (intptr_t)memoryPoolHeader.startPoint + POOL_SIZE) {
            ((MemoryBlockHeader *)((intptr_t) freeBlockHeader->body + freeBlockHeader->size))->prevBlockPoint = freeBlockHeader;
        }
    }
}
