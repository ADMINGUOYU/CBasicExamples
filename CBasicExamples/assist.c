#include "assist.h"

int get_int(void)
{
	int input;
	while (scanf("%d", &input) != 1)
	{
		while (getchar() != '\n') {}
		printf("Invalid Input >> Try again\n");
	}
	while (getchar() != '\n') {}
	return input;
}

int inputNUMRange(int min, int max) 
{
	int num;
	int flag = 0;
	while (!flag) {
		num = get_int();
		if (num < min || num > max) {
			flag = 0;
			printf("Out of range [%d,%d] >> Try again\n", min, max);
		}
		else {
			flag = 1;
		}
	}
	return num;
}

size_t get_size_t(void)
{
	size_t input;
	while (scanf("%zd", &input) != 1)
	{
		while (getchar() != '\n') {}
		printf("Invalid Input >> Try again\n");
	}
	while (getchar() != '\n') {}
	return input;
}