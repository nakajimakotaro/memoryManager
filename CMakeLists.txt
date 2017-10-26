cmake_minimum_required(VERSION 3.8)
project(memoryManager)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        source/memoryManager.c
        )
set(TEST_FILES
        source/test.cpp
        )
set(TEST_LIBRARY_PATH
        library/googletest-release-1.8.0/googlemock/gtest/
        )
set(TEST_INCLUDE_PATH
        library/googletest-release-1.8.0/googlemock/include/
        )
set(TEST_LINK_LIBRARY
        gtest
        pthread
        )

include_directories(${TEST_INCLUDE_PATH})
link_directories(${TEST_LIBRARY_PATH})
add_executable(memoryManager ${SOURCE_FILES} ${TEST_FILES})
target_link_libraries(memoryManager ${TEST_LINK_LIBRARY})