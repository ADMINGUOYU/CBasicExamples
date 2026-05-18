// Dynamic Programming
// Minimum Steps To 1

/*
On a positive integer, you can perform any one of the following 3 steps. 
1.) Subtract 1 from it ( n <- n - 1 ). 
2.) If its divisible by 2, divide by 2. ( n <- n / 2 if n % 2 == 0 ).  
3.) If its divisible by 3, divide by 3. ( n <- n / 3 if n % 3 == 0 ). 
Given a positive integer n, find the minimum number of steps that takes n to 1
Examples:
1.) For n=1, output: 0       
2.) For n=4, output: 2  (4 /2=2 /2=1 )    
3.) For n=7, output: 3  (7 -1=6 /3=2 /2=1 )
Design a Greedy and a Dynamic Programming algorithm for the problem. 

NOTE: Greedy algorithm is not optimal for this problem, 
      Dynamic Programming algorithm has O(n) time complexity and O(n) space complexity.
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Word definition of actions
#define ACTION_INVALID 0
#define ACTION_SUBTRACT_1 1
#define ACTION_DIVIDE_2 2
#define ACTION_DIVIDE_3 3
static char * action_to_string (int action)
{
    switch (action)
    {
        case ACTION_SUBTRACT_1: return "Subtract 1";
        case ACTION_DIVIDE_2: return "Divide by 2";
        case ACTION_DIVIDE_3: return "Divide by 3";
        case ACTION_INVALID: return "Invalid Action";
        default: return "Unknown Action";
    }
}

// Minimum steps to one result structure
typedef struct minimum_steps_to_one_result
{
    // Minimum steps to 1
    int min_steps;
    // Actions taken to achieve minimum steps
    // Records actions taken at each step
    // e.g. actions_taken[0] = ACTION_DIVIDE_2 means the first action taken is to divide by 2
    // NOTE: buffer size is n - 1 (to ONE), but only the first min_steps entries are valid
    //       we use 0 to indicate invalid entry
    int * actions_taken;
} minimum_steps_to_one_result;

// Greedy algorithm for minimum steps to one
static minimum_steps_to_one_result greedy_minimum_steps_to_one(int n)
{
    // GREEDY ALGORITHM:
    // -> we choose the action that reduces n the most at each step,
    //    without considering future consequences.
    // 1.) If n is divisible by 3, divide by 3.
    // 2.) Else if n is divisible by 2, divide by 2.
    // 3.) Else, subtract 1 from n.

    // Initialize result structure
    minimum_steps_to_one_result result = 
    { .min_steps = 0, .actions_taken = NULL };

    // Allocate memory for actions taken
    // (worst case is n - 1 steps, all subtract 1)
    result.actions_taken = (int *) malloc((n - 1) * sizeof(int));
    for (int i = 0; i < n - 1; ++i) result.actions_taken[i] = ACTION_INVALID;

    // Perform greedy algorithm
    int current_n = n;
    int step = 0;
    while (current_n > 1)
    {
        // 1) Check if divisible by 3
        if (current_n % 3 == 0)
        {
            current_n /= 3;
            result.actions_taken[step] = ACTION_DIVIDE_3;
        }
        // 2) Check if divisible by 2
        else if (current_n % 2 == 0)
        {
            current_n /= 2;
            result.actions_taken[step] = ACTION_DIVIDE_2;
        }
        // 3) Else, subtract 1
        else
        {
            current_n -= 1;
            result.actions_taken[step] = ACTION_SUBTRACT_1;
        }
        ++step;
    }
    // Set the minimum steps in the result structure
    result.min_steps = step;

    return result;
}

// Dynamic programming algorithm for minimum steps to one
static minimum_steps_to_one_result dp_minimum_steps_to_one(int n)
{
    // For recursive approach, we try all options and choose the one
    // with minimum steps, now, we do the bottom-up.

    // we take:
    // min( 1 + last(action_1), 1 + last(action_2), 1 + last(action_3))

        // Prepare result structure
    minimum_steps_to_one_result result = { .min_steps = 0, .actions_taken = NULL };

    // CHECK if n is <= 1, if so, return 0 steps
    if (n <= 1) return result;

    // Initialize MIN_STEP array
    int * MIN_STEP = (int * ) malloc ((n + 1) * sizeof(int));
    MIN_STEP[0] = 0; MIN_STEP[1] = 0;
    for (int i = 2; i <= n; ++i) MIN_STEP[i] = INT_MAX;

    // Initialize BACK_TRACK array
    int * BACK_TRACK = (int * ) malloc ((n + 1) * sizeof(int));
    for (int i = 0; i <= n; ++i) BACK_TRACK[i] = ACTION_INVALID;
    
    // Main LOOP
    for (int i = 2; i <= n; ++i)
    {
        // 1) Check if divisible by 3
        if (i % 3 == 0 && MIN_STEP[i / 3] + 1 < MIN_STEP[i])
        {
            MIN_STEP[i] = MIN_STEP[i / 3] + 1;
            BACK_TRACK[i] = ACTION_DIVIDE_3;
        }
        // 2) Check if divisible by 2
        if (i % 2 == 0 && MIN_STEP[i / 2] + 1 < MIN_STEP[i])
        {
            MIN_STEP[i] = MIN_STEP[i / 2] + 1;
            BACK_TRACK[i] = ACTION_DIVIDE_2;
        }
        // 3) Else, subtract 1
        if (MIN_STEP[i - 1] + 1 < MIN_STEP[i])
        {
            MIN_STEP[i] = MIN_STEP[i - 1] + 1;
            BACK_TRACK[i] = ACTION_SUBTRACT_1;
        }
    }

    // Set the minimum steps in the result structure
    result.min_steps = MIN_STEP[n];
    // Allocate memory for actions taken
    result.actions_taken = (int *) malloc(result.min_steps * sizeof(int));
    // Backtrack to find the actions taken
    int current_n = n;
    int step = 0;
    while (current_n > 1)
    {
        result.actions_taken[step] = BACK_TRACK[current_n];
        switch (BACK_TRACK[current_n])
        {
            case ACTION_DIVIDE_3:
                current_n /= 3;
                break;
            case ACTION_DIVIDE_2:
                current_n /= 2;
                break;
            case ACTION_SUBTRACT_1:
                current_n -= 1;
                break;
        }
        ++step;
    }

    // Free allocated memory
    free(MIN_STEP);
    free(BACK_TRACK);

    return result;
}

// Entry point
int alg_dp_minimum_steps_to_one_main(int argc, char* argv[])
{
    // Set N to a test value
    int N = 10;
    // Run some test cases for greedy algorithm
    minimum_steps_to_one_result greedy_result = greedy_minimum_steps_to_one(N);
    // Run some test cases for dynamic programming algorithm
    minimum_steps_to_one_result dp_result = dp_minimum_steps_to_one(N);

    // Print the result
    printf("\033[1;32m>>> Minimum Steps To One >>>\033[0m\n");
    printf("  ├── \033[1mGreedy Step Count:\033[0m %d\n", greedy_result.min_steps);
    printf("  ├── \033[1mGreedy Actions Taken:\033[0m\n");
    for (int i = 0; i < greedy_result.min_steps; ++i)
    {
        if (i < greedy_result.min_steps - 1) printf("  │   ├── ");
        else printf("  │   └── ");
        printf("%s\n", action_to_string(greedy_result.actions_taken[i]));
    }
    printf("  ├── \033[1mDP Step Count:\033[0m %d\n", dp_result.min_steps);
    printf("  └── \033[1mDP Actions Taken:\033[0m\n");
    for (int i = 0; i < dp_result.min_steps; ++i)
    {        if (i < dp_result.min_steps - 1) printf("    ├── ");
        else printf("    └── ");
        printf("%s\n", action_to_string(dp_result.actions_taken[i]));
    }

    // Free allocated memory for actions taken
    free(greedy_result.actions_taken);
    free(dp_result.actions_taken);

    return 0;
}