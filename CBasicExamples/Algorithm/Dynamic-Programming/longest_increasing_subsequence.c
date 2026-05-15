// Dynamic Programming
// Longest Monotonically Increasing Subsequence

/*
For example, if the input sequence is [5, 24, 8, 17, 12, 45],
the output should be either
[5, 8, 12, 45] or [5, 8, 17, 45]

Solution:

[Optimal sub-structure]:
LIS[X] = LENGTH of longest sequence that ENDs at Xth element
ALL initially 1 (each element can be a sequence of length 1 by itself)

[Recurrence]:
LIS[X] = max{ LIS[a] where 0 <= a < X AND value[a] < value[X] } + 1

[Final answer]:
max{ LIS[i] } (tracked within the DP loop)

[Tracing the answer array]:
BACK_TRACE[X] = argmax{ LIS[a] where 0 <= a < X AND value[a] < value[X] }

[Complexity]:
Recurrence loop O(n^2)
Answer tracing O(n)
Overall O(n^2)
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Result structure
typedef struct longest_increasing_subsequence_result
{
    // length of answer
    int length;
    // answer array
    // buffer size = N
    int * sequence;
} longest_increasing_subsequence_result;

// Solver
static longest_increasing_subsequence_result
longest_increasing_subsequence (int * array, int N)
{
    // Initialize the LIS array
    int * LIS = (int *) malloc (N * sizeof(int));
    for (int i = 0; i < N; ++i) LIS[i] = 1;

    // Initialize the BACK_TRACE array
    int * BACK_TRACE = (int *) malloc (N * sizeof(int));
    for (int i = 0; i < N; ++i) BACK_TRACE[i] = -1;

    // main loop
    int max_length[2] = { 1, 0 }; // {length, idx}
    for (int i = 1; i < N; ++i)
    {
        // loop through all previous elements to find the
        // longest increasing subsequence that can be extended by array[i]
        for (int a = 0; a < i; ++a)
        {
            // if array[a] < array[i],
            // we can extend the subsequence that ends at a by array[i]
            if (array[a] < array[i])
            {
                int candidate_length = LIS[a] + 1;
                if (candidate_length > LIS[i])
                {
                    LIS[i] = candidate_length;
                    BACK_TRACE[i] = a;
                }
            }
            // else we cannot extend the subsequence
        }
        
        // update max_length
        if (LIS[i] > max_length[0])
        {
            max_length[0] = LIS[i];
            max_length[1] = i;
        }        
    }

    // Prepare the result
    longest_increasing_subsequence_result result;
    result.length = max_length[0];
    result.sequence = (int *) malloc (result.length * sizeof(int));
    // Backtrack the answer
    int current_idx = max_length[1];
    for (int i = result.length - 1; i >= 0; --i)
    {
        result.sequence[i] = array[current_idx];
        current_idx = BACK_TRACE[current_idx];
    }

    // Free the allocated memory
    free(LIS);
    free(BACK_TRACE);

    // Return the result
    return result;
}

// Entry point
int alg_dp_longest_increasing_subsequence_main(int argc, char* argv[])
{
    // Initialize the input array
    int array[] = { 5, 24, 8, 17, 12, 45 };
    int N = sizeof(array) / sizeof(array[0]);

    // Get the result
    longest_increasing_subsequence_result result = longest_increasing_subsequence(array, N);

    // Print the result
    printf("\033[1;32m>>> Longest Increasing Subsequence [RESULT] >>>\033[0m\n");
    printf("  ├── \033[1mInput array:\033[0m ");
    for (int i = 0; i < N; ++i) printf("%02d ", array[i]);
    printf("\n");
    printf("  ├── \033[1mLength of longest increasing subsequence:\033[0m %d\n", result.length);
    printf("  └── \033[1mLongest increasing subsequence:\033[0m ");
    for (int i = 0; i < result.length; ++i) printf("%02d ", result.sequence[i]);
    printf("\n");

    // Free the allocated memory
    free(result.sequence);

    return 0;
}