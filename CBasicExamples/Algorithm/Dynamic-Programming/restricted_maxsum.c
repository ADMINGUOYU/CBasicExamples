// Dynamic Programming
// The (Restricted) Max-Sum Problem

/*
Let A be a sequence of n numbers a1, a2, . . . , an.
Find a subset S of A that has the maximum sum, provided that,
    if a_{i} ∈ S, then a_{i−1} ∉ S and a_{i+1} ∉ S.
Note that, A is a sequence in which order matters (and not an unordered set).
    (the ordering will restrict the selection based on the above rule.)
As an example, if A = 1, 8, 6, 3, 7, the max possible sum is S = {8, 7}

NOTE:
    To implement it using normal recursive function, we do:
    (Assume our function could solve [a1, ... , a_{i-1}],
    and we are at the i-th element.)
    -> If a[i] in the collection, then a[i-1] should NOT be considered,
       We then call our function to solve for a[1 ... i-2]
    -> If a[i] is NOT in the collection, then we should further solve
       the problem on series a[1 ... i-1]
    !!! Even though the restriction is NO (i-1) AND (i+1) if (i) is selected,
        the relation is relative hence solvable just considering the previous.
    Our function will get the collection from both calls and PLUS a[i] for the
    (i) considering group. We do the comparision and select the larger solution
    to be assigned for position a[i].

To optimize the above recursion:
 - Remember LARGEST-SUM[i] as largest sum we can get at i;

!!! Actually, largest_sum_including is NOT necessary,
    from the code below, you see that we only index largest_sum_including[i]
    NEVER used previous elements.
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Result structure
typedef struct restricted_maxsum_result
{
    // maximum sum we can get
    int max_sum;
    // length of the selected subset
    int length;
    // array of indices of the selected elements in the original array
    // buffer size = N
    int * selected_indices;
} restricted_maxsum_result;

// Solver
static restricted_maxsum_result
restricted_maxsum (int * array, int N)
{
    // Initialize the DP arrays
    int * largest_sum_including = (int *) malloc (N * sizeof(int));
    int * largest_sum = (int *) malloc (N * sizeof(int));
    char * CHOICE = (char *) malloc (N * sizeof(char)); // 1 if we choose to include a[i], 0 otherwise

    // Base cases
    largest_sum_including[0] = array[0];
    // Handle if a[0] < 0, VERY IMPORTANT
    largest_sum[0] = (array[0] > 0) ? array[0] : 0;
    CHOICE[0] = (array[0] > 0) ? 1 : 0;

    // Main loop
    for (int i = 1; i < N; ++i)
    {
        // If we include a[i], we cannot include a[i-1]
        // NOTE: if i-2<0 for a[1], we directly assign 0 -> nothing is selected
        largest_sum_including[i] = array[i] + ((i - 2 >= 0) ? largest_sum[i - 2] : 0);
        // If we do not include a[i], we take the largest sum from the previous element
        largest_sum[i] = largest_sum[i - 1];

        // Decide whether to include a[i] or not
        if (largest_sum_including[i] > largest_sum[i])
        {
            // We include a[i]
            CHOICE[i] = 1;
            largest_sum[i] = largest_sum_including[i];
        }
        else
        {
            // We do not include a[i]
            CHOICE[i] = 0;
        }
    }

    // Backtrack to find the selected indices
    int max_sum = largest_sum[N - 1];
    int length = 0;
    int * selected_indices = (int *) malloc (N * sizeof(int)); // temporary buffer for selected indices
    for (int i = N - 1; i >= 0; )
    {
        if (CHOICE[i] == 1)
        {
            selected_indices[length++] = i; // store the index of the selected element
            i -= 2; // skip the adjacent element
        }
        else
        {
            i -= 1; // move to the previous element
        }
    }

    // Reverse the selected indices to get them in the original order
    for (int i = 0; i < length / 2; ++i)
    {
        int temp = selected_indices[i];
        selected_indices[i] = selected_indices[length - i - 1];
        selected_indices[length - i - 1] = temp;
    }

    // Prepare the result
    restricted_maxsum_result final_result = 
    { 
        .max_sum = max_sum, 
        .length = length, 
        .selected_indices = selected_indices
    };

    // Free the allocated memory for the temporary arrays
    free(largest_sum_including);
    free(largest_sum);
    free(CHOICE);

    return final_result;
}

// Entry point
int alg_dp_restricted_maxsum_main(int argc, char* argv[])
{
    // Initialize the input array
    int array[] = { 1, 8, 6, 3, 7 };
    int N = sizeof(array) / sizeof(array[0]);

    // Get the result
    restricted_maxsum_result result = restricted_maxsum(array, N);

    // Print the result
    printf("\033[1;32m>>> Restricted Max-Sum Problem [RESULT] >>>\033[0m\n");
    printf("  ├── \033[1mInput array:\033[0m ");
    for (int i = 0; i < N; ++i)
    {
        if (i < N - 1) printf("%d, ", array[i]);
        else printf("%d\n", array[i]);
    }
    printf("  ├── \033[1mMaximum sum:\033[0m %d\n", result.max_sum);
    printf("  ├── \033[1mSelected indices in the original array:\033[0m ");
    if (result.length == 0) printf("None\n");
    else
    for (int i = 0; i < result.length; ++i)
    {
        if (i < result.length - 1) printf("%d, ", result.selected_indices[i]);
        else printf("%d\n", result.selected_indices[i]);
    }
    printf("  └── \033[1mSelected elements in the original array:\033[0m ");
    if (result.length == 0) printf("None\n");
    else
    for (int i = 0; i < result.length; ++i)
    {
        if (i < result.length - 1) printf("%d, ", array[result.selected_indices[i]]);
        else printf("%d\n", array[result.selected_indices[i]]);
    }

    // Free the allocated memory for selected_indices
    free(result.selected_indices);

    return 0;
}