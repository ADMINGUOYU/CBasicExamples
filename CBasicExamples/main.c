//Include from STD library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Include DEMO functions
#include "funcList.h"
//Define MACROS
#define FuncCALL (*myFunc)(argc, argv)
#define FuncDef(X) myFunc = X
//Declaration of external functions with file wide linkage
extern int get_int(void);
extern int inputNUMRange(int min, int max);
//Function prototyping (Forward Declaration)
int menu(void);
void programChooser(void);
//Declaration of function pointer
int (*myFunc)(int argc, char* argv[]);

//Program ENTRY --> Main Function
int main(int argc,char* argv[]) {

	int go = 1;
	while (go) {
		menu();
		programChooser();
		do {
			printf("\n");
			printf("-------------------------------< Function Execution Starts >\n");
			FuncCALL;
			printf("-------------------------------<Function Execution Completes>\n");
			printf("\n<Repeat?> \n[ENTER] to repeat\nPress any key to quit\n");
		} while (getchar() == '\n');

		while(getchar() != '\n'){}

		printf("\n<Continue?> \n[ENTER] to continue\nPress any key to quit\n");
		if (getchar() != '\n') {
			go = 0;
		}
	}
	
	return 0;
}

//Definition of function "menu"
int menu(void) {

	printf("********************************\n");
	printf("*** This is a DEMO C program ***\n");
	printf("********************************\n");
	printf("\n");
	
	printf("Here is the DEMO program list >>\n");
	printf("\n");
	for (int i = 0; i < ProgramCount; i++) {
		printf("%-3d: %s\n", i, programList[i]);
	}
	printf("\n");
	printf("--> %d program(s) in total\n", ProgramCount);
	printf("\n");

	return 0;
}

//Definition of function "programChooser"
void programChooser(void) {
	int select;
	printf("Please select: ");
	select = inputNUMRange(0, ProgramCount - 1);
	FuncDef(programFunc[select]);
}