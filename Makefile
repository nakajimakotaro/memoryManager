
test: $(CFILE)
	g++ -Llibrary/googletest-release-1.8.0/googlemock/gtest -Ilibrary/googletest-release-1.8.0/googlemock/include/ source/test.cpp source/memoryManager.c -lgtest -pthread

library: $(CFILE)
	clang -c source/memoryManager.c -o source/memoryManager.o
	ar -r build/memoryManager.a source/memoryManager.o

