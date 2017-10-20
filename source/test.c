#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <stdio.h>
#include "memoryManager.h"

void allocateTest(){
    debugPoolView();
    CU_ASSERT(memoryPoolHeader.freeMemoryList->size == POOL_SIZE - 32);

    myAllocate(10);
    CU_ASSERT(memoryPoolHeader.freeMemoryList->size == POOL_SIZE - 32 - 10 - 32);
    debugPoolView();

    myAllocate(30);
    CU_ASSERT(memoryPoolHeader.freeMemoryList->size == POOL_SIZE - 32 - 10 - 32 - 30 - 32);
    debugPoolView();

    myAllocate(32);
    CU_ASSERT(memoryPoolHeader.freeMemoryList->size == POOL_SIZE - 32 - 10 - 32 - 30 - 32 - 32 - 32);
    debugPoolView();

    myAllocate(1);
    CU_ASSERT(memoryPoolHeader.freeMemoryList->size == 0);
    debugPoolView();
}

int main(){
    CU_initialize_registry();
    CU_pSuite test_suite = CU_add_suite("myAllocate", NULL, NULL);
    initMemory();
    CU_add_test(test_suite, "allocateTest", allocateTest);
    CU_console_run_tests();
    CU_cleanup_registry();
    return 0;
}
void debugPoolView(){
    printf("poolStart: %ld\n", (size_t)memoryPoolHeader.startPoint);
    printf("poolLast:  %ld\n", (size_t)((int8_t*)memoryPoolHeader.startPoint + POOL_SIZE));
    printf("freePoint: %ld\n", (size_t)memoryPoolHeader.freeMemoryList);
    printf("freeSize:  %ld\n", (size_t)memoryPoolHeader.freeMemoryList->size);
    printf("\n");
}