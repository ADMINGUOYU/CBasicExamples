#define _CRT_SECURE_NO_WARNINGS 1
#include "memoryAlloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assist.h"

extern int get_int(void);
extern int inputNUMRange(int min, int max);
extern size_t get_size_t(void);

int memAlloc(size_t size);

int memAllocMain(int argc, char* argv[]) {
	size_t size = 0;
	if (argc > 1) {
		size = (size_t)argv[1];
	}
	printf("Welcome to memory allocation\n");
	if (size == 0) {
		printf("Please enter size to allocate >> ");
		size = get_size_t();
	}
	//while (getchar() != '\n') {}
	printf("Program will allocate %zd byte(s) of memory\nPress [ENTER] to continue >>\n", size);
	while (getchar() != '\n') {}
	memAlloc(size);

	return 0;
}

int memAlloc(size_t size) {
	void* ptr = malloc(size);
	if (ptr == NULL) {
		printf("%s\n", strerror(errno));
		printf("[Operation FAILED] program will terminate.\n");
		return EXIT_FAILURE;
	}
	printf("Memory allocation complete, memory starts at: %p\n",ptr);
	printf("Press [ENTER] to free memory >>");
	while (getchar() != '\n') {}
	free(ptr);
	ptr = NULL;
	printf("[Operation COMPLETE] program will terminate.\n");
	return 0;
}