#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include <stdio.h>
#include <inttypes.h>
#include "memoryManager.h"

void allocateTest(){
    initMemory();
    debugPoolView();
    CU_ASSERT(memoryPoolHeader.freeMemoryList->size == POOL_SIZE - 32);

    void* m1 = myAllocate(10);
    CU_ASSERT(memoryPoolHeader.freeMemoryList->size == POOL_SIZE - 32 - 10 - 32);
    debugPoolView();

    void* m2 = myAllocate(30);
    CU_ASSERT(memoryPoolHeader.freeMemoryList->size == POOL_SIZE - 32 - 10 - 32 - 30 - 32);
    debugPoolView();

    myFree(m2);
    CU_ASSERT(memoryPoolHeader.freeMemoryList->size == POOL_SIZE - 32 - 10 - 32);
    debugPoolView();

    void* m5 = myAllocate(30);
    CU_ASSERT(memoryPoolHeader.freeMemoryList->size == POOL_SIZE - 32 - 10 - 32 - 30 - 32);
    debugPoolView();


}

int main(){
    CU_initialize_registry();
    CU_pSuite test_suite = CU_add_suite("myAllocate", NULL, NULL);
    CU_add_test(test_suite, "allocateTest", allocateTest);
    CU_console_run_tests();
    CU_cleanup_registry();
    return 0;
}
void debugPoolView(){
    printf("poolStart: %zu\n", (size_t)memoryPoolHeader.startPoint);
    printf("poolLast:  %zu\n", (size_t)((intptr_t)memoryPoolHeader.startPoint + POOL_SIZE));
    printf("freePoint: %zu\n", (size_t)memoryPoolHeader.freeMemoryList);
    printf("freeSize:  %zu\n", memoryPoolHeader.freeMemoryList->size);
    printf("\n");
}
