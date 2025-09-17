#ifndef _ASSIST_FILEIO_H_
#define _ASSIST_FILEIO_H_

typedef struct fileIO fileIO;

void fileIO_init(fileIO* file,char* path);
int fileOpen(fileIO* file,char* mode);
int fileClose(fileIO* file);
int fileWrite(fileIO* file, char* content);

#endif