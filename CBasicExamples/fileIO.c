#define _CRT_SECURE_NO_WARNINGS 1
#include "fileIO.h"
#include <stdio.h>
#include <stdlib.h>

#define CNTL_Z '\032'   /* eof marker in DOS text files */
#define SLEN 81

//Forward Declarations
int fileLetterCounter(int argc, char* argv[]);
int fileReverse(void);

//Main function
int fileIOMain(int argc, char* argv[]) {
    fileReverse();

	return 0;
}

//Definition of function "fileLetterCounter"
int fileLetterCounter(int argc, char* argv[]) {
    int ch;         // place to store each character as read
    FILE* fp;       // "file pointer"
    unsigned long count = 0;

    if (argc != 2)
    {
        printf("Usage: %s filename\n", argv[0]);
        return EXIT_FAILURE;
    }
    if ((fp = fopen(argv[1], "r")) == NULL)
    {
        printf("Can't open %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    while ((ch = getc(fp)) != EOF)
    {
        putc(ch, stdout);  // same as putchar(ch);
        count++;
    }
    fclose(fp);
    printf("File %s has %lu characters\n", argv[1], count);

    return 0;
}

//Definition of function "fileReverse"
int fileReverse(void)
{
    char file[SLEN];
    char ch;
    FILE* ofp = NULL, * fp;
    long count, last;
    int outFlag = 0;

    printf("**This program will print the given file's characters in a reversed order**\n\n");
    
    printf("Would you like to save a copy? (y/n) >> ");
    if (getchar() == 'y') { 
        printf(">> Find output.txt in the same directory as executable <<\n");
        ofp = fopen("output.txt", "w+");
        outFlag = 1; 
    }
    while (getchar() != '\n'){}
    
    puts("Enter the name of the file to be processed:");
    scanf("%80s", file);
    if ((fp = fopen(file, "rb")) == NULL)
    {                               /* read-only mode   */
        printf("reverse can't open %s\n", file);
        printf("read-only mode\n");
        //remove leftover characters
        while (getchar() != '\n') {}

        return EXIT_FAILURE;
    }
    putchar('\n');

    fseek(fp, 0L, SEEK_END);        /* go to end of file */
    last = ftell(fp);
    printf("<< Now we will begin printing >>\n\n");
    for (count = 1L; count <= last; count++)
    {
        fseek(fp, -count, SEEK_END); /* go backward      */
        ch = getc(fp);
        if (ch != CNTL_Z && ch != '\r') { /* MS-DOS files */
            putchar(ch);
            if (outFlag) { fprintf(ofp,"%c",ch); }
        }  
    }
    putchar('\n');
    putchar('\n');
    printf("<< Printing ends HERE >>\n");
    fclose(fp);

    //remove leftover characters
    while(getchar() != '\n'){}

    return 0;
}