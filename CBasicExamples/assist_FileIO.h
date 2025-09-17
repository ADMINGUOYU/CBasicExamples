#ifndef _ASSIST_FILEIO_H_
#define _ASSIST_FILEIO_H_

#include <stdio.h>
#include <string.h>

#define MAX_NAME_LENGTH 90

typedef struct fileIO fileIO;

void fileIO_init(fileIO* file,char* path);
int fileOpen(fileIO* file,char* mode);
int fileClose(fileIO* file);
int fileWrite(fileIO* file, char* content);

#endif