#define _CRT_SECURE_NO_WARNINGS 1
#include "funcPointer.h"
#include <stdio.h>
#include <ctype.h>
#define MAX 100

void (*ptrFUNC) (char*);
void ToUpper(char*);
void ToLower(char*);
void Default(char*);

int funcPointerMain(int argc, char* argv[])
{
    char buff[MAX] = { 0 };
    char option = '0';

    puts("This program uses a function pointer to execute designated method");
    puts("Please input a string >>");
    gets(buff);

    puts("Please specify an action: \n[U] --> to UPPER\n[l] --> to lower\n[Other Key] --> do nothing\n>> ");
    scanf("%c", &option);
    while (getchar() != '\n') {}

    if (option == 'U')
    {
        ptrFUNC = ToUpper;
    }
    else if (option == 'l')
    {
        ptrFUNC = ToLower;
    }
    else
    {
        ptrFUNC = Default;
    }

    (*ptrFUNC)(buff);
    puts(buff);

    return 0;
}


void ToUpper(char* tmp)
{
    int i;
    puts("We are converting your input to UPPER case...");
    for (i = 0; i < MAX, tmp[i] != '\0'; i++)
    {
        tmp[i] = toupper(tmp[i]);
    }
    return;
}
void ToLower(char* tmp)
{
    int i;
    puts("We are converting your input to lower case...");
    for (i = 0; i < MAX, tmp[i] != '\0'; i++)
    {
        tmp[i] = tolower(tmp[i]);
    }
    return;
}
void Default(char* tmp) {
    puts("We will not mod your input...");
    return;
}