#include "memoryManager.h"
#include "gtest/gtest.h"

void debugPoolView(){
    printf("poolStart: %zu\n", (size_t)memoryPoolHeader.startPoint);
    printf("poolLast:  %zu\n", (size_t)((intptr_t)memoryPoolHeader.startPoint + POOL_SIZE));
    printf("freePoint: %zu\n", (size_t)memoryPoolHeader.freeMemoryList);
    printf("freeSize:  %zu\n", memoryPoolHeader.freeMemoryList->size);
    printf("\n");
}

TEST(memory, memory){
    initMemory();
    debugPoolView();
    ASSERT_TRUE(memoryPoolHeader.freeMemoryList->size == POOL_SIZE - 40);

    void* m1 = myAllocate(10);
    ASSERT_TRUE(memoryPoolHeader.freeMemoryList->size == POOL_SIZE - 40 - 10 - 40);
    debugPoolView();

    void* m2 = myAllocate(30);
    ASSERT_TRUE(memoryPoolHeader.freeMemoryList->size == POOL_SIZE - 40 - 10 - 40 - 30 - 40);
    debugPoolView();

    myFree(m1);
    ASSERT_TRUE(memoryPoolHeader.freeMemoryList->size == POOL_SIZE - 40 - 10 - 40 - 30 - 40 + 10);
    debugPoolView();

    void* m5 = myAllocate(30);
    ASSERT_TRUE(memoryPoolHeader.freeMemoryList->size == POOL_SIZE - 40 - 10 - 40 - 30 - 40);
    debugPoolView();
}

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
