// 2D Dynamic Programming
// 0/1 Knapsack Problem

/*
Input: A set of 𝑛 items,
       where item 𝑖 has weight 𝑤𝑖 and value 𝑣𝑖,
       and a knapsack with capacity 𝑊.
Goal:  Find 𝑥1 , … , 𝑥𝑛 ∈ {0,1} satisfying σ(𝑥𝑖*𝑤𝑖) ≤ 𝑊
       that maximizes σ(𝑥𝑖*𝑣𝑖).

NOTE: brute force: O(2^n) time complexity (each item has 2 choices: include or exclude), 
                   O(n) space complexity
      dynamic programming: 
                   O(n*W) time complexity, 
                   O(n*W) space complexity

Why Greedy Algorithm Fails?
!!! Greedy solution:
    We sort items by value, weight, or density (value/weight)
    and select items based on that order until we reach the knapsack's
    capacity.
 - If we sort items by value and select the most valuable items first,
   (consider density = value/weight), we might end up with a total weight
   that exceeds the knapsack's capacity, resulting in a suboptimal
   solution. (Adding the most valuable item is not a good idea for all)

Why 1D Dynamic Programming Fails?
!!! 1D DP solution:
    Let 𝑉[𝑤] be the largest obtainable value for a knapsack with
    capacity 𝑤.
    If Optimal Solution for knapsack of size w chooses item 𝑖,
    remainder of optimal solution is optimal solution for subproblem
    of filling knapsack of size 𝑤 − 𝑤𝑖
    (similar to 1D solution coin denominations - minimum_number_of_coins)

    𝑉[𝑤] = max(0, 𝑣1 +𝑉[𝑤 − 𝑤1], 𝑣2 + 𝑉[𝑤 − 𝑤2], … , 𝑣𝑛 +𝑉[𝑤 − 𝑤𝑛])
    𝑉[𝑗] = 0, 𝑗 ≤ 0

!!! However: 
    1D DP solution does not work for 0/1 knapsack problem because it does not
    account for the fact that each item can only be **included once**. 
    The 1D DP approach assumes that we can include an item multiple times, 
    which is not the case in the 0/1 knapsack problem. 
    Therefore, we need to use a 2D DP approach to properly handle the 
    inclusion and exclusion of items.

Brute-force + recursion method (similar to game tree search):
    We can represent the problem as a binary tree where each node represents a
    decision to include or exclude an item. (like huffman coding tree)
    The depth of the tree is equal to the number of items, and the branches
    represent the two choices (include or exclude). 
    This approach explores all possible combinations of items, leading to a
    time complexity of O(2^n). [number of leaf nodes in the tree is 2^n]

SO, what does 2D-Dynamic Programming adds?
-> Keep track of what has been used and what has not been used
 - We can define a 2D DP table where the rows represent the items
   and the columns represent the capacities from 0 to W.
 - The value in the table at row i and column w represents the maximum
   value that can be obtained with the **first i items** and a 
   knapsack **capacity of w**.
Recurrence: 𝑉[𝑖, 𝑤] = max(𝑉[𝑖 − 1, 𝑤], 𝑣𝑖 +𝑉[𝑖 − 1, 𝑤 − 𝑤𝑖])
            ∀𝑖,𝑉[𝑖, 0] = 0
-> Thus we can ensure that each item is only included once by only looking
   at the previous row (i-1) when making our decision for the current item
   (i).
!!! No need to sort the items !!!
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Item structure
typedef struct item
{
    int weight;
    int value;
} item;

// Knapsack result structure
typedef struct knapsack_result
{
    // Maximum value that can be obtained with the given capacity
    int max_value;
    // Capacity of the knapsack (max weight it can hold)
    int capacity;
    // array of item indices included in the knapsack
    // Length = n
    // if items_included[i] == 1, it means item i is included
    // if items_included[i] == 0, it means item i is not included
    char * items_included;
} knapsack_result;

// 2D DP solution for 0/1 Knapsack problem
static knapsack_result knapsack_0_1(const item * items, int n, int W)
{
    /*
    Argument explanation:
    items : array of items (weight and value)
    n     : len(items)
    W     : capacity of the knapsack
    */

    // Initialize the DP table
    int ** V = (int **) malloc ((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; ++i) V[i] = (int *) malloc ((W + 1) * sizeof(int));
    for (int i = 0; i <= n; ++i)
        for (int w = 0; w <= W; ++w)
            V[i][w] = 0;

    // Initialize KEEP flag table
    int ** KEEP = (int **) malloc ((n + 1) * sizeof(int *));
    for (int i = 0; i <= n; ++i) KEEP[i] = (int *) malloc ((W + 1) * sizeof(int));
    for (int i = 0; i <= n; ++i)
        for (int w = 0; w <= W; ++w)
            KEEP[i][w] = 0;

    // Fill the DP table
    for (int weight = 1; weight <= W; ++weight)
        // for each weight value we try different amount of items
        for (int item = 1; item <= n; ++item)
        {
            // Get the weight of this item
            int itm_weight = items[item - 1].weight;
            // Get the value of this item
            int itm_value = items[item - 1].value;

            // PREV value (if this item is not included)
            int prev_value = V[item - 1][weight];

            // Check if it's a good idea to include this item
            if ((itm_weight <= weight) &&
                ((itm_value + V[item - 1][weight - itm_weight]) > prev_value))
            {
                // update value if we include this item
                V[item][weight] = itm_value + V[item - 1][weight - itm_weight];
                // we include this item
                KEEP[item][weight] = 1;
            }
            else
                V[item][weight] = prev_value;
        }

    // [DEBUG] print the DP table
    printf("\033[1;34m>>> DP Table (V) >>>\033[0m (including row 0 and column 0)\n");
    for (int i = 0; i <= n; ++i)
    {
        for (int w = 0; w <= W; ++w)
            printf("%4d ", V[i][w]);
        printf("\n");
    }
    printf("\n");
    // [DEBUG] print the KEEP table
    printf("\033[1;34m>>> KEEP Table >>>\033[0m (including row 0 and column 0)\n");
    for (int i = 0; i <= n; ++i)
    {
        for (int w = 0; w <= W; ++w)
            printf("%4d ", KEEP[i][w]);
        printf("\n");
    }
    printf("\n");

    // Backtrack to find which items are included
    char * items_included = (char *) malloc (n * sizeof(char));
    for (int i = 0; i < n; ++i) items_included[i] = 0; // initialize to 0 (not included)
    int w = W;
    for (int item = n; item > 0; --item)
    {
        if (KEEP[item][w] == 1)
        {
            // This item is included
            items_included[item - 1] = 1; // mark this item as included
            w -= items[item - 1].weight; // decrease the remaining weight
        }
    }

    // Prepare the result
    knapsack_result result;
    result.max_value = V[n][W];
    result.capacity = W;
    result.items_included = items_included;

    // Free the allocated memory for DP tables
    for (int i = 0; i <= n; ++i) free(V[i]);
    free(V);
    for (int i = 0; i <= n; ++i) free(KEEP[i]);
    free(KEEP);

    // Return the result
    return result;
}

// Entry point
int alg_dp_2d_0_1_knapsack_main(int argc, char* argv[])
{
    // Initialize items
    item items[] = {
        // {weight, value}
        {5, 10},
        {4, 40},
        {6, 30},
        {3, 50}
    };

    // Number of items and knapsack capacity
    int n = sizeof(items) / sizeof(items[0]);
    int W = 10;

    // Solve the knapsack problem
    knapsack_result result = knapsack_0_1(items, n, W);

    // Print the result
    printf("\033[1;32m>>> 0/1 Knapsack Problem [RESULT] >>>\033[0m\n");
    printf("  ├── \033[1mKnapsack capacity (W):\033[0m %d\n", result.capacity);
    printf("  ├── \033[1mItems (weight, value):\033[0m\n");
    for (int i = 0; i < n; ++i)
    {
        if (i < n - 1) printf("  │   ├── ");
        else printf("  │   └── ");
        printf("[%02d] Weight: %02d, Value: %02d\n", (i + 1), items[i].weight, items[i].value);
    }
    printf("  ├── \033[1mMaximum value:\033[0m %d\n", result.max_value);
    printf("  └── \033[1mItems included in the knapsack (indices):\033[0m ");
    for (int i = 0; i < n; ++i)
    {
        if (result.items_included[i] == 1) printf("%02d; ", i + 1);
    }
    printf("\n");

    // Free the allocated memory for items_included
    free(result.items_included);

    return 0;
}