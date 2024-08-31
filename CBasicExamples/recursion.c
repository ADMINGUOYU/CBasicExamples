#include "recursion.h"

extern int get_int(void);
extern int inputNUMRange(int min, int max);

//global variables
int i = 0;

//function prototyping
void up_and_down(int n);

//Main function
int recursionMain(int argc, char* argv[]) {

	int recursionTimes = 0;
	if (argc > 1) {
		recursionTimes = argv[1];
	}
	printf("Here shows an example of Recursion\n");
	if (recursionTimes == 0) {
		printf("Please enter recursion times >> ");
		recursionTimes = get_int();
	}
	printf("Set to %d time(s)\nPress [ENTER] to start >>\n", recursionTimes);
	while(getchar() != '\n') {}
	up_and_down(recursionTimes);

	return 0;
}

void up_and_down(int n)
{
	i++;
	printf("Level %d: variable location %p\n", i, &n); /* 1 */
	if (i < n)
		up_and_down(n);
	printf("LEVEL %d: variable location %p\n", i, &n); /* 2 */
}