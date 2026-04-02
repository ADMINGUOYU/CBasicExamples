// Dynamic-Programming
// Rod Cutting Problem

/*
Given a rod of length n and prices p_{i} for i = 1,2,...,n, 
where p_{i} is the price of a rod of length i. 
Find a way to cut the rod to maximize total revenue.

NOTE: brute force: O(2^n) time complexity (n - 1 places to cut), 
                   O(n) space complexity
      dynamic programming: O(n^2) time complexity, O(n) space complexity
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Rot cut result
typedef struct rod_cut_result
{
    // Maximum revenue
    int max_revenue;
    // Cuts to achieve maximum revenue
    // Records lengths of cuts
    // e.g. cuts[0] = 3 means the first cut is of length 3
    // NOTE: assign buffer size to n
    int * cuts;
} rod_cut_result;

// Rod cutting function
rod_cut_result rod_cutting(int price[], int n)
{
    // Initialize memory array -> bottom-up best result cache
    int * VALUE = (int * )malloc((n + 1) * sizeof(int));
    // Initialize all to 0
    for (int i = 0; i <= n; ++i) VALUE[i] = 0;
    // Initialize cut back tracing
    int * cuts = (int * )malloc((n + 1) * sizeof(int));
    for (int i = 0; i <= n; ++i) cuts[i] = 0;

    // Main processing loop
    for (int length = 1; length <= n; ++length)
    {
        // Bottom-up method -> Build from n = 1
        
        // Initialize best weight
        int best = INT_MIN;
        // we loop through all possible cut location
        for (int cut = 1; cut <= length; ++cut)
        {
            // fetch weight for this cut
            int weight = price[cut - 1]; // -1 to convert from 1-indexed to 0-indexed
            // fetch remaining weight for the other piece
            int remaining_value = VALUE[length - cut]; // length - cut is the remaining length after the cut

            // Do the comparison
            if ((weight + remaining_value) > best)
            {
                // Update best
                best = (weight + remaining_value);
                // Update cut info
                cuts[length] = cut;
            }
        }
        // Update best revenue
        VALUE[length] = best;
    }

    // Initialize the result struct
    rod_cut_result result;
    result.max_revenue = VALUE[n];
    result.cuts = (int * )malloc(n * sizeof(int));
    // Initialize cuts to 0
    for (int i = 0; i < n; ++i) result.cuts[i] = 0;

    // Backtracking to fetch the cuts
    int remaining_length = n;
    int cut_index = 0;
    while (remaining_length > 0)
    {
        // Fetch the cut length for the current remaining length
        int cut_length = cuts[remaining_length];
        // Store the cut length in the result
        result.cuts[cut_index++] = cut_length;
        // Update remaining length
        remaining_length -= cut_length;
    }

    // Free the memory allocated for VALUE and cuts
    free(VALUE);
    free(cuts);

    // Return the result
    return result;
}

// NOTE: recursive approach:
// int rod_cutting_recursive(int price[], int n)
// {
//     // Base case: no rod left
//     if (n == 0) return 0;
//     // Initialize best revenue
//     int best = INT_MIN;
//     // Loop through all possible cut locations
//     for (int cut = 1; cut <= n; ++cut)
//     {
//         // Calculate revenue for this cut
//         int revenue = price[cut - 1] + rod_cutting_recursive(price, n - cut);
//         // Update best revenue         
//         if (revenue > best) best = revenue;
//     }
//     return best;

// Program entry point
int alg_dp_rod_cutting_main (int argc, char * argv[])
{
    // Initialize the price array and the length of the rod
    // price[i] is the price of a rod of length i
    int price[] = {1, 5, 8, 9, 10, 17, 17, 20, 24};

    // Set the length of the rod
    int n = sizeof(price) / sizeof(price[0]);

    // Call the rod cutting function
    rod_cut_result result = rod_cutting(price, n);
    
    // Print the result
    printf("\033[1;32m>>> Rod Cutting Problem [RESULT] >>>\033[0m\n");
    printf("  ├── \033[1mRod length:\033[0m %d\n", n);
    printf("  ├── \033[1mPrice array:\033[0m ");
    for (int i = 0; i < n; ++i) printf("len(%d):$%d ", i + 1, price[i]);
    printf("\n");
    printf("  ├── \033[1mMaximum revenue:\033[0m $%d\n", result.max_revenue);
    printf("  └── \033[1mCuts:\033[0m ");
    for (int i = 0; i < n; ++i)
    {
        // if cuts[i] is 0, it means no more cuts
        if (result.cuts[i] == 0) break;
        // Print the cut length
        printf("chunk(%d):%d ", i + 1, result.cuts[i]);
    }
    printf("\n");

    // Free the memory allocated for cuts
    free(result.cuts);
    return 0;
}