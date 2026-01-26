#include "array_demo.h"
#include <stdio.h>

#define ARR_CONTENT { 6, 4, 8 }

int array_demo_main(int argc, char *argv[])
{
    puts(">>> Welcome to C array demonstration <<<");
    puts("Now, we'll create an 'array of integers' first ... ");
    fflush(stdout);

    int array[] = ARR_CONTENT;
    int length = sizeof(array) / sizeof(int);

    printf("-> the created array has length of %d, int array[%d] = {", length, length);
    for (int i = 0; i < length; ++i)
    {
        if (i != 0) printf(",");
        printf(" %d", array[i]);
    }
    puts(" }");
    fflush(stdout);

    printf("-> the created array starts at [%p], its members occupy:\n", &array);
    for (int i = 0; i < length; ++i)
        printf("\tarray[%d] = %d (%p)\n", i, array[i], (array + i));
    fflush(stdout);

    return 0;
}