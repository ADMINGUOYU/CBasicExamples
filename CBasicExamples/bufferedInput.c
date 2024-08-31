#include "bufferedInput.h"

int echo(void);

int bufferedInputMain(int argc, char* argv[]) {
	echo();
	return 0;
}

int echo(void) {
	char ch;
	printf("This piece of program demonstrates input echo.\n");
	printf("The buffer (stdin) is flushed when [ENTER] is pressed\n");
	printf("Now input something and press [ENTER] >>\n");
	printf("--> Type '#' to quit\n");
	//Here starts the sequence
	while ((ch = getchar()) != '#')
		putchar(ch);
	while((ch = getchar()) != '\n'){}
	//Here ends the sequence
	return 0;
}