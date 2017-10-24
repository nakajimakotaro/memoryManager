
CC=clang
test: $(CFILE)
	$(CC) source/memoryManager.c source/test.c -g  -o test.out

library: $(CFILE)
	$(CC) -c source/memoryManager.c -o source/memoryManager.o
	ar -r build/memoryManager.a source/memoryManager.o

