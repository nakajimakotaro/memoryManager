extern "C"{
    #include "memoryManager.h"
}
#include "gtest/gtest.h"

void debugPoolView(){
    MemoryBlockHeader* blockHeader = memoryPoolHeader.startPoint;
    printf("\n");
    while((intptr_t)blockHeader < (intptr_t)(memoryPoolHeader.startPoint) + POOL_SIZE){
        if(blockHeader->isUse){
            printf("■");
        }else{
            printf("□");
        }

        blockHeader = (MemoryBlockHeader*)((intptr_t)blockHeader + sizeof(MemoryBlockHeader) + blockHeader->size);
    }
    printf("\n");
    blockHeader = memoryPoolHeader.startPoint;
    while((intptr_t)blockHeader < (intptr_t)(memoryPoolHeader.startPoint) + POOL_SIZE){
        printf("blockStart %p\n", blockHeader);
        printf("isUse      %s\n", blockHeader->isUse ? "true" : "false");
        printf("blockSize  %zu\n", (size_t)blockHeader->size);
        blockHeader = (MemoryBlockHeader*)((intptr_t)blockHeader + sizeof(MemoryBlockHeader) + blockHeader->size);
    }
    printf("\n");
}

TEST(memory, memory2){
    initMemory();
    debugPoolView();
    ASSERT_EQ(memoryPoolHeader.freeMemoryList->size, POOL_SIZE - 40);

    void* m1 = myAllocate(10);
    debugPoolView();
    ASSERT_EQ(memoryPoolHeader.freeMemoryList->size, POOL_SIZE - 40 - 10 - 40);

    void* m2 = myAllocate(10);
    debugPoolView();
    ASSERT_EQ(memoryPoolHeader.freeMemoryList->size, POOL_SIZE - 40 - 10 - 40 - 10 - 40);

    void* m3 = myAllocate(10);
    debugPoolView();
    ASSERT_EQ(memoryPoolHeader.freeMemoryList->size, POOL_SIZE - 40 - 10 - 40 - 10 - 40 - 10 - 40);
    void* m4 = myAllocate(10);
    debugPoolView();

    myFree(m1);
    debugPoolView();
    myFree(m2);
    debugPoolView();
    myFree(m3);
    m3 = myAllocate(10);
    debugPoolView();
    myFree(m3);
    debugPoolView();
    m4 = myAllocate(10);
    debugPoolView();
    myFree(m4);
    debugPoolView();
    //ASSERT_EQ(memoryPoolHeader.freeMemoryList->size, POOL_SIZE - sizeof(MemoryBlockHeader));
}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
