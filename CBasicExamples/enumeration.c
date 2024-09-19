#include "enumeration.h"
#include <stdio.h>

const int weekdaysCount = 7;
enum weekdays {Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday};

int enumerationMain(int argc, char* argv[]) {
	printf("Now We will print the names of weekdays:\n");

	printf("%-13s -> %d\n", "Monday", Monday);
	printf("%-13s -> %d\n", "Tuesday", Tuesday);
	printf("%-13s -> %d\n", "Wednesday", Wednesday);
	printf("%-13s -> %d\n", "Thursday", Thursday);
	printf("%-13s -> %d\n", "Friday", Friday);
	printf("%-13s -> %d\n", "Saturday", Saturday);
	printf("%-13s -> %d\n", "Sunday", Sunday);

	return 0;
}