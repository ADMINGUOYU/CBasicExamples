// Dynamic-Programming
// Minimum number of Coins

/*
Given a value V AND an array of coin denominations coins[] of size m,
find the minimum number of coins that you need to make change for V.
NOTE: brute force: O(m^V) time complexity (each '1' in V has m choices), 
                   O(V) space complexity
      dynamic programming: 
                   O(m*V) time complexity, 
                   O(V) space complexity
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// min-coin result structure
typedef struct min_coin_result
{
    // Minimum number of coins
    // NOTE: min_coins == -1 for no solution
    int min_coins;
    // Coins used to achieve minimum number of coins
    // Records denominations of coins used
    // e.g. coins_used[0] = 25 means the first coin used is of denomination 25
    // NOTE: size of coins_used is min_coins
    int * coins_used;
} min_coin_result;

// Minimum number of coins function
static min_coin_result minimum_number_of_coins(int coins[], int m, int V)
{
    /*
    Argument explanation:
    coins[]: possible denominations
    m      : len(coins)
    V      : target amount
    */

    // Bottom-up: build up solution from V = 1

    // Initialize list of values (we use 1-based indexing)
    // COIN_CNT[1] -> minimum count when V = 1
    int * COIN_CNT = (int *) malloc((V + 1) * sizeof(int));
    COIN_CNT[0] = 0;
    for (int i = 1; i <= V; ++i) COIN_CNT[i] = INT_MAX;

    // Initialize answer trace back list
    // TRACE_BACK[1] -> choose TRACE_BACK[1] denomination when V = 1
    int * TRACE_BACK = (int *) malloc((V + 1) * sizeof(int));
    for (int i = 0; i <= V; ++i) TRACE_BACK[i] = INT_MIN;

    // Main loop
    for (int value = 1; value <= V; ++value)
    {
        // NOTE: COIN_CNT[value] -> minimum coin count when V = value

        // Test all coins
        for (int coin_idx = 0; coin_idx < m; ++coin_idx)
        {
            // Get denomination of this coin
            int denomination = coins[coin_idx];

            // Calculate remaining value
            int remaining_value = value - denomination;

            // Break if the note is too large
            if (remaining_value < 0) continue;

            // Get minimum coin count with remaining_value
            int remaining_value_coin_count = COIN_CNT[remaining_value];

            // Get total coin count (1 + remaining_value_coin_count)
            int total_coin_count = 1 + remaining_value_coin_count;

            // Test if this is the better solution
            if (total_coin_count < COIN_CNT[value])
            {
                // Update COIN_CNT
                COIN_CNT[value] = total_coin_count;
                // Update TRACE_BACK
                TRACE_BACK[value] = denomination;
            }
        }
    }

    // Now prepare the result
    min_coin_result result;
    result.min_coins = (COIN_CNT[V] == INT_MAX) ? -1 : COIN_CNT[V];

    // If we have a solution, prepare the coins used list
    if (result.min_coins != -1)
    {
        // Allocate memory for coins_used list
        result.coins_used = (int *) malloc(result.min_coins * sizeof(int));

        // Trace back to get the coins used
        int current_value = V;
        for (int i = 0; i < result.min_coins; ++i)
        {
            // Get the denomination of the coin used for current_value
            int denomination = TRACE_BACK[current_value];
            result.coins_used[i] = denomination;
            // Update current_value
            current_value -= denomination;
        }
    }
    else
    {
        // No solution, set coins_used to NULL
        result.coins_used = NULL;
    }

    // Free the allocated memory
    free(COIN_CNT);
    free(TRACE_BACK);

    return result;
}

// Entry point
int alg_dp_minimum_number_of_coins_main(int argc, char* argv[])
{
    // Initialize the coin denominations and the value V
    int coins[] = {25, 10, 1};
    int m = sizeof(coins) / sizeof(coins[0]);
    // Initialize V (target value)
    int V = 30;

    // Get the result
    min_coin_result result = minimum_number_of_coins(coins, m, V);

    // Print the result
    printf("\033[1;32m>>> Minimum Number of Coins Problem [RESULT] >>>\033[0m\n");
    printf("  ├── \033[1mValue (V):\033[0m ¢%d\n", V);
    printf("  ├── \033[1mCoin denominations:\033[0m ");
    for (int i = 0; i < m; ++i) printf("¢%d ", coins[i]);
    printf("\n");
    printf("  ├── \033[1mMinimum number of coins:\033[0m %d\n", result.min_coins);
    printf("  └── \033[1mCoins used:\033[0m ");
    for (int i = 0; i < result.min_coins; ++i) printf("coin(%d):¢%d ", i + 1, result.coins_used[i]);
    printf("\n");

    // Free the allocated memory
    free(result.coins_used);

    return 0;
}