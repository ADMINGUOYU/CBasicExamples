#define _CRT_SECURE_NO_WARNINGS 1
#include "assist_FileIO.h"
#include <stdio.h>
#include <string.h>


#define MAX_NAME_LENGTH 90

typedef struct fileIO {
	FILE* fptr;
	char filePath[MAX_NAME_LENGTH];
}fileIO;

void fileIO_init(fileIO* file,char* path) {
	file->fptr = NULL;
	strncpy(file->filePath, path, MAX_NAME_LENGTH);
	return;
}

int fileOpen(fileIO* file,char* mode) {
	file->fptr = fopen(file->filePath, mode);
	if (file->fptr == NULL) {
		printf("fileIO ERROR: failed to open file.\n");
		return -1;
	}
	return 0;
}

int fileClose(fileIO* file) {
	if (file->fptr == NULL) {
		printf("fileIO ERROR: file pointer NULL.\n");
		return -1;
	}
	fclose(file->fptr);
	file->fptr == NULL;
	return 0;
}

int fileWrite(fileIO* file, char* content) {
	if (file->fptr == NULL) {
		printf("fileIO ERROR: file pointer NULL.\n");
		return -1;
	}
	fprintf(file->fptr,"%s",content);
	return 0;
}