#include "assist_IO.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	while (scanf("%zu", &input) != 1)
	{
		while (getchar() != '\n') {}
		printf("Invalid Input >> Try again\n");
	}
	while (getchar() != '\n') {}
	return input;
}

size_t inputNUMRange_size_t(size_t min, size_t max) 
{
	size_t num;
	char flag = 0;
	while (!flag) {
		num = get_size_t();
		// NOTE: unsigned type will never be less than 0
		//       if min == 0, the first condition will always be false
		if (num < min || num > max) {
			flag = 0;
			printf("Out of range [%zu,%zu] >> Try again\n", min, max);
		}
		else {
			flag = 1;
		}
	}
	return num;
}