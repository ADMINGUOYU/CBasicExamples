#define _CRT_SECURE_NO_WARNINGS 1
#include "memoryAlloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assist_IO.h"

#define AUTO_NEW_LINE 20

extern int get_int(void);
extern int inputNUMRange(int min, int max);
extern size_t get_size_t(void);

void* memAlloc(size_t size);
int memInit(void* ptr, size_t size);
int memFree(void* ptr);
int memPrint(void* ptr, size_t size);
int memWrite(void* ptr, int pos, size_t size, char* content, int count);

int memAllocMain(int argc, char* argv[]) {
	//memory alloc
	void* ptr;
	//input buffer
	char* writePtr;
	//write flag
	int writeEnabled = 1;

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

	//Allocate memory for both 'memory' and 'input buffer'
	ptr = memAlloc(size);
	writePtr = (char*)malloc(size);

	//Error checking
	if (ptr == NULL) { return -1; }
	if (writePtr == NULL) {
		printf("ERROR: can't write!\n"); 
		writeEnabled = 0;
	}

	while (1) {
		printf("Select operation >>\ni) Initialize memory\np) Print memory\nw) Write memory\nf) Free memory and quit\n\n");

		switch (getchar())
		{
		case 'i':
			//Get excessive '\n' in input buffer
			while (getchar() != '\n') {}

			//init mem (using function 'memInit')
			memInit(ptr, size);

			//Just for BETTER looking
			putchar('\n');
			break;
		case 'p':
			//Get excessive '\n' in input buffer
			while (getchar() != '\n') {}

			//Print the memory out (using function "memPrint")
			memPrint(ptr, size);

			//Just for BETTER looking
			putchar('\n');
			break;
		case 'w':
			//Get excessive '\n' in input buffer
			while (getchar() != '\n') {}

			//Check if buffer to read keyboard input is ready
			if (writeEnabled == 0) { printf("ERROR: can't write!\n"); break; }

			//Get memory write start location (pointer offset)
			int pos;
			printf("Insert position >> ");
			pos = inputNUMRange(0, size - 1); 

			//Get input from keyboard
			int count = 0;
			printf("INPUT >> ");
			while ((writePtr[count] = getchar()) != '\n' && count < size) { count++; }	//Just ignore warnings 'C6054' and 'C6386'
			writePtr[count] = '\0';
			printf("Press [ENTER] again to finish input.\n");
			while (getchar() != '\n') {}

			//Write input to memory (using function "memWrite")
			if (memWrite(ptr, pos, size, writePtr, count) == 0) { printf("Done.\n"); }
			putchar('\n');
			break;
		case'f':	//Using C switch pitfall mechanism
		default:
			//Get excessive '\n' in input buffer
			while (getchar() != '\n') {}

			//Free memory
			memFree(ptr);
			ptr == NULL;

			//Free input buffer
			if (writePtr != NULL) {
				free(writePtr);
				writePtr = NULL;
			}
			return 0;
		}
	}

}

//Definition of funtion "memAlloc" -> allocate memory
void* memAlloc(size_t size) {
	void* ptr = malloc(size);
	if (ptr == NULL) {
		printf("%s\n", strerror(errno));
		printf("[Operation FAILED] program will terminate.\n");
		return NULL;
	}
	printf("Memory allocation complete, memory starts at: %p\n",ptr);
	return ptr;
}

//Definition of funtion "memInit"
int memInit(void* ptr, size_t size) {
	char* cptr = (char*)ptr;
	
	//Error checking
	if (ptr == NULL) {
		printf("ERROR: NULL pointer.\n");
		return -1;
	}

	for (int i = 0; i < size; i++) {
		cptr[i] = ' ';
	}

	printf("Done.\n");

	return 0;
}

//Definition of funtion "memFree" -> frees allocated memory blocks
int memFree(void* ptr) {
	//Error checking
	if (ptr == NULL) {
		printf("ERROR: NULL pointer.\n");
		return -1;
	}
	printf("Press [ENTER] to free memory >>");
	while (getchar() != '\n') {}

	//Free memory (using standard library function)
	free(ptr);

	printf("[Operation COMPLETE] program will terminate.\n");
	return 0;
}

//Definition of funtion "memPrint" -> prints contents as characters in allocated memory blocks
int memPrint(void* ptr,size_t size) {
	char* cptr = (char*)ptr;
	
	//Error checking
	if (ptr == NULL) {
		printf("ERROR: NULL pointer.\n");
		return -1;
	}

	printf("Printing content in memory (as character): \n");
	putchar('\n');

	//Print first row [column marks]
	for (int i = 0; i <= AUTO_NEW_LINE; i++) {
		printf("|");
		printf("%3d", i);
	}
	putchar('\n');

	//Start printing memory blocks [with row marks]
	for (int i = 0; i < (int)size; i++) {
		if (i == 0) {
			printf("|");
			printf("%3s", "1");
		}
		printf("|");
		printf("%3c", cptr[i]);
		if ((i + 1) % AUTO_NEW_LINE == 0) {
			putchar('\n'); 
			printf("|");
			printf("%3d", ((i + 1) / AUTO_NEW_LINE) + 1);
		}
	}

	//Just for BETTER looking
	putchar('\n');

	return 0;
}

int memWrite(void* ptr, int pos, size_t size, char* content,int count) {
	char* cptr = (char*)ptr;

	//Error checking
	if (count > (size - pos)) {
		printf("ERROR: not enough space to write content.\n");
		return -1;
	}

	//Start writing
	for (int i = 0; i < count; i++) {
		cptr[pos + i] = content[i];
	}

	return 0;
}