#pragma once  
#include <stdbool.h>
#include <stdint.h>

typedef struct _MemoryBlockHeader{
    struct _MemoryBlockHeader* prevFreeBlock;
    struct _MemoryBlockHeader* nextFreeBlock;
    size_t size;
    bool isUse;
    void* body[];
} MemoryBlockHeader;
typedef struct _MemoryPoolHeader{
    void* startPoint;
    MemoryBlockHeader* freeMemoryList;
} MemoryPoolHeader;
void* myAllocate(size_t size);
void* myFree(void* ptr);
void initMemory();
void debugPoolView();
extern MemoryPoolHeader memoryPoolHeader;
extern const size_t POOL_SIZE;