// 2D Dynamic Programming
// Egg Dropping Problem

/*
The egg dropping problem is a classic algorithmic puzzle aimed at
finding the highest floor (n) from which an egg can be dropped
without breaking, using a specific number of eggs (N) and building
floors (F). 
The goal is to determine the minimum number of drops required in the
worst-case scenario to guarantee finding the threshold floor.

IDEA:
1. If we have only one egg (N = 1), we must drop it from each
   floor starting from the first until it breaks, resulting in F
   drops in the worst case.
2. If we have more than one egg (N > 1), we can drop an egg from
   a certain floor (x). If the egg breaks, we need to check the
   floors below (x-1) with one less egg (N-1). If it doesn't break,
   we need to check the floors above (F-x) with the same number of
   eggs (N).

The recursive relation can be expressed as:
D(N, F) = 1 + min_{1 <= x <= F} (max(D(N-1, x-1), D(N, F-x)))
Where:
- D(N, F) is the minimum number of drops required with N eggs and F floors
- The "1 +" accounts for the drop we are making at floor x
- The "max" function accounts for the worst-case scenario (whether
  the egg breaks or not at floor x)
- The "min" function finds the optimal floor x to drop the egg from
  to minimize the number of drops in the worst case.
The base cases are:
- D(1, F) = F (with one egg, we have to try every floor)
- D(N, 0) = 0 (with zero floors, no drops are needed)
- D(N, 1) = 1 (with one floor, only one drop is needed)
if N = 0, D(0, F) = 0 (with zero eggs, no drops are needed)

Running time:
The time complexity of the naive recursive solution is O(F^N) due to
the exponential growth of the recursive calls.
However, using dynamic programming to store previously computed results
can reduce the time complexity to O(N * F^2) in the worst case,
as we compute the solution for each combination of N and F once, and
for each combination, we iterate through all possible sub-floors (x)
to find the optimal drop point.
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Result structure
typedef struct egg_dropping_result
{
    // Minimum number of drops required in the worst case
    int drops;
    // Optimal floor first drop
    int optimal_floor;

} egg_dropping_result;

// 2D DP solution for egg dropping problem
egg_dropping_result egg_dropping(int N, int F)
{
    // create a 2D DP table
    // D[EGG][FLOOR]
    int ** D = (int **) malloc ((N + 1) * sizeof(int *));
    for (int i = 0; i <= N; ++i)
        D[i] = (int *) malloc ((F + 1) * sizeof(int));

    // create a 2D table to store the optimal floor for
    // each combination of N and F
    int ** optimal_floors = (int **) malloc ((N + 1) * sizeof(int *));
    for (int i = 0; i <= N; ++i)
        optimal_floors[i] = (int *) malloc ((F + 1) * sizeof(int));

    // Base cases
    for (int f = 0; f <= F; ++f)
    {
        D[1][f] = f; // With one egg, we need f drops for f floors
        optimal_floors[1][f] = 1; // Optimal floor is the first floor for one egg
    }
    for (int n = 1; n <= N; ++n)
    {
        D[n][0] = 0; // With zero floors, no drops are needed
        D[n][1] = 1; // With one floor, only one drop is needed
        optimal_floors[n][0] = 0; // Optimal floor is the first floor for zero floors
        optimal_floors[n][1] = 1; // Optimal floor is the first floor for one floor
    }
    // If we have 0 eggs...
    for (int f = 0; f <= F; ++f)
    {
        D[0][f] = 0; // With zero eggs, no drops are needed
        optimal_floors[0][f] = 0; // Optimal floor is the first floor for zero eggs
    }

    // Fill the DP table using the recursive relation
    for (int n = 2; n <= N; ++n)
    {
        for (int f = 2; f <= F; ++f)
        {
            D[n][f] = INT_MAX; // Initialize to a large number

            // Try dropping from each floor x and take the minimum
            // of the worst-case drops
            for (int x = 1; x <= f; ++x)
            {
                // We choose the maximum of the two sections
                int drops = 1 + 
                           (D[n - 1][x - 1] > D[n][f - x] ? 
                            D[n - 1][x - 1] : D[n][f - x]);
                // Update the minimum drops for D[n][f]
                if (drops < D[n][f])
                {
                    D[n][f] = drops;
                    optimal_floors[n][f] = x;
                }
            }
        }
    }

    // [DEBUG] Print the DP table
    printf("\033[1;34m>>> DP Table (D[N][F]) >>>\033[0m\n");
    // Print header
    printf("\033[1m%5s\033[0m", "N\\F");
    for (int f = 0; f <= F; ++f)
        printf("\033[1m%5d\033[0m", f);
    printf("\n");
    // Print rows
    for (int n = 0; n <= N; ++n)
    {
        printf("\033[1m%5d\033[0m", n);
        for (int f = 0; f <= F; ++f)
            printf("%5d", D[n][f]);
        printf("\n");
    }

    // [DEBUG] Print the optimal floors table
    printf("\033[1;34m>>> Optimal Floors Table (optimal_floors[N][F]) >>>\033[0m\n");
    // Print header
    printf("\033[1m%5s\033[0m", "N\\F");
    for (int f = 0; f <= F; ++f)
        printf("\033[1m%5d\033[0m", f);
    printf("\n");
    // Print rows
    for (int n = 0; n <= N; ++n)
    {
        printf("\033[1m%5d\033[0m", n);
        for (int f = 0; f <= F; ++f)
            printf("%5d", optimal_floors[n][f]);
        printf("\n");
    }

    // The result is in D[N][F]
    int result_drops = D[N][F];
    int result_optimal_floor = optimal_floors[N][F];

    // Free the allocated memory
    for (int i = 0; i <= N; ++i)
    {
        free(D[i]);
        free(optimal_floors[i]);
    }
    free(D);
    free(optimal_floors);

    // Return the result
    return (egg_dropping_result) { .drops = result_drops, .optimal_floor = result_optimal_floor };
}

// Entry point
int alg_dp_2d_egg_dropping_main(int argc, char* argv[])
{
    // Initialize the number of eggs and floors
    int N = 3; // number of eggs
    int F = 10; // number of floors

    // Compute the result using the egg dropping function
    egg_dropping_result result = egg_dropping(N, F);

    // Print the result
    printf("\033[1;32m>>> Egg Dropping N*F^2 solution [RESULT] >>>\033[0m\n");
    printf("  ├── \033[1mNumber of eggs (N):\033[0m %d\n", N);
    printf("  ├── \033[1mNumber of floors (F):\033[0m %d\n", F);
    printf("  ├── \033[1mOptimized drops:\033[0m %d\n", result.drops);
    printf("  └── \033[1mOptimal drop floor:\033[0m %d\n", result.optimal_floor);

    return 0;
}