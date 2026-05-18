// Dynamic Programming (TOP DOWN with memorization)
// Longest Increasing Path in a Matrix

/*
Consider a two-dimensional table A[1..n, 1..n] of distinct integers.
We want to find the longest increasing path in A. A sequence of
table cells:
    A[i_1 , j_1], A[i_2 , j_2], ... , A[i_k , j_k], A[i_{k+1} , j_{k+1}], ...
is a path in A if and only if every pair of consecutive cells in the
sequence share a side (horizontal or vertical).

A path in A is increasing if and only if:
    A[i_1 , j_1] < A[i_2 , j_2] < ... < A[i_k , j_k] < A[i_{k+1} , j_{k+1}]
The length of a path is the number of cells in it.

Brute-force approach:
We can start from each cell and perform a depth-first search (DFS) to find
the longest increasing path starting from that cell. This approach has a
time complexity of O(n^2 * 4^k) in the worst case, where n is the size of
the matrix and k is the length of the longest path.
(n^2 for starting from each cell,
and 4^k for exploring all possible paths: up, down, left, right)

Optimal substructure:
If we have a longest increasing path starting from a cell A[i, j], then
the next cell in the path must be one of the four adjacent cells
(up, down, left, right) that has a value greater than A[i, j].
This leads to a recursive formulation that can be solved using dynamic
programming.
We can define a DP table where the value at cell (i, j) represents the
length of the longest increasing path starting from cell A[i, j].
The longest increasing path in the entire matrix will be the maximum value
in this DP table.

Recurrence:
Let DP[i][j] be the length of the longest increasing path starting from cell A[i, j].
Then:
DP[i][j] = 1 + max(DP[i-1][j], DP[i+1][j], DP[i][j-1], DP[i][j+1]) 
if A[i][j] < A[i-1][j], A[i][j] < A[i+1][j], A[i][j] < A[i][j-1], A[i][j] < A[i][j+1])
DP[i][j] = 1, if there are no adjacent cells with greater value than A[i][j]
* NOTE: We can also let DP[i][j] be the length of the longest increasing path ENDING at
cell A[i, j], and the recurrence would be similar but in reverse direction.

We initialize the DP table with -1 to indicate that the value has not been computed yet. 
When we compute DP[i][j], we check the four adjacent cells and recursively compute their 
DP values if they have not been computed yet.

Pseudocode:
[This procedure runs n^2 times -> each cell is computed once]
Procedure longest_increasing_path(A[1 ... n, 1 ... n]):
    Initialize DP table with -1
    max_length <- 0
    for i from 1 to n DO:
        for j from 1 to n DO:
            DP[i][j] <- longest_increasing_path_from_cell(A, i, j, DP)
            max_length <- max(max_length, DP[i][j])
    return max_length

[This procedure is called n^2 times,
but each cell is computed once due to memorization;
EACH cell's longest path is computed in O(1) time - 4 adjacent cells to check;
EACH cell's retrieval is O(1) time due to memorization]
Procedure longest_increasing_path_from_cell(A[1 ... n, 1 ... n], i, j, DP):
    if DP[i][j] != -1: # DP memorization check
        return DP[i][j]
    DP[i][j] <- 1 # The cell itself counts as a path of length 1
    for each (next_i, next_j) in [(i-1, j), (i+1, j), (i, j-1), (i, j+1)] DO: # ALL 4 directions
        if next_i and next_j are within bounds of A AND A[i][j] < A[next_i][next_j]:
            DP[i][j] <- max(DP[i][j], 1 + longest_increasing_path_from_cell(A, next_i, next_j, DP))
    return DP[i][j]

Running time: [each cell ONLY computed once]
The time complexity of this approach is O(n^2) because we compute the longest
path starting from each cell at most once, and each cell's longest path is computed
in O(1) time after the initial computation (due to memorization).

NOTE: We can turn this into a bottom-up approach by filling the DP table iteratively.
      However, we have to do sorting.
      [Instead of making A[i][j] be the length of the longest path STARING at cell (i, j),
       we can make A[i][j] be the length of the longest path ENDING at cell (i, j).
       Then we can fill the DP table in increasing order of A[i][j] values.]
      
    - We pre-fill the DP table with 1s (each cell itself is a path of length 1),
      and then we sort the cells by their values in A. <- O(n^2 log(n^2)) = O(n^2 log n)
      
    - Then we iterate through the sorted cells and update the DP values based on the adjacent cells.
      This makes sure that when we process a cell, all the cells with smaller values have already
      been processed, and we can use their DP values to compute the current cell's DP value.
      
    - The way of filling the DP table is similar to the top-down approach, as well as
      how we backtrace the longest path.

    - Time complexity of the bottom-up approach is O(n^2 log n) due to sorting,
      and O(n^2) for filling the DP table.

    - Overall time complexity is O(n^2 log n) for the bottom-up approach.
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Result structure
typedef struct longest_increasing_path_result
{
    // Length of the longest increasing path
    int length;
    // The longest increasing path (array of cell coordinates)
    // Length of the path is given by 'length'
    // Each cell coordinate is represented as a pair of integers (i, j)
    int (* path)[2]; // pointer to an array of pairs of integers
} longest_increasing_path_result;

// Helper function to compute the longest increasing path starting from cell (i, j)
static int longest_increasing_path_from_cell(const int ** A, int n, int i, int j, int ** DP, int (* path_table)[2])
{

    /*
    Argument explanation:
    A          : 2D array of distinct integers (matrix)
    n          : size of the matrix (n x n)
    i, j       : current cell coordinates
    DP         : DP table to store the longest path length starting from each cell
    path_table : 2D array to store the next cell in the longest path for backtracking
    */

    // If the longest path from this cell has already been computed, return it
    if (DP[i][j] != -1) return DP[i][j];

    // Initialize the longest path length from this cell to 1 (the cell itself)
    DP[i][j] = 1;

    // Check the four adjacent cells (up, down, left, right)
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // up, down, left, right
    for (int d = 0; d < 4; ++d)
    {
        int next_i = i + directions[d][0];
        int next_j = j + directions[d][1];
        // Check if the adjacent cell is within bounds and has a greater value
        if (next_i >= 0 && next_i < n && 
            next_j >= 0 && next_j < n && 
            A[next_i][next_j] > A[i][j])
        {
            int path_length = 1 + longest_increasing_path_from_cell(A, n, next_i, next_j, DP, path_table);
            if (path_length > DP[i][j])
            {
                DP[i][j] = path_length;
                // Update the path table to point to the next cell in the longest path
                path_table[i * n + j][0] = next_i; // next cell's i coordinate
                path_table[i * n + j][1] = next_j; // next cell's j coordinate
            }
        }
    }
    return DP[i][j];
}

// DP solution for longest increasing path in a matrix
static longest_increasing_path_result longest_increasing_path(const int ** A, int n)
{
    /*
    Argument explanation:
    A : 2D array of distinct integers (matrix)
    n : size of the matrix (n x n)
    */

    // Initialize the DP table
    int ** DP = (int **) malloc (n * sizeof(int *));
    for (int i = 0; i < n; ++i)
        DP[i] = (int *) malloc (n * sizeof(int));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            DP[i][j] = -1;

    // Initialize the path table to store the next cell in the longest path
    int (* path_table)[2] = (int (*)[2]) malloc (n * n * sizeof(int[2]));
    for (int i = 0; i < n * n; ++i)
    {
        path_table[i][0] = -1; // next cell's i coordinate
        path_table[i][1] = -1; // next cell's j coordinate
    }

    // Compute the longest increasing path starting from each cell
    int max_length = 0;
    int start_i = 0, start_j = 0; // starting cell of the longest path
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
        {
            int path_length = longest_increasing_path_from_cell(A, n, i, j, DP, path_table);
            if (path_length > max_length)
            {
                max_length = path_length;
                start_i = i;
                start_j = j;
            }
        }

    // Backtrack to find the longest increasing path
    int (* longest_path)[2] = (int (*)[2]) malloc (max_length * sizeof(int[2]));
    int path_index = 0;
    int current_i = start_i, current_j = start_j;
    while (current_i != -1 && current_j != -1)
    {
        longest_path[path_index][0] = current_i;
        longest_path[path_index][1] = current_j;
        int next_i = path_table[current_i * n + current_j][0];
        int next_j = path_table[current_i * n + current_j][1];
        current_i = next_i;
        current_j = next_j;
        ++path_index;
    }

    // Prepare the result
    longest_increasing_path_result result;
    result.length = max_length;
    result.path = longest_path;

    // Free the allocated memory for DP tables
    for (int i = 0; i < n; ++i) free(DP[i]);
    free(DP);

    // Free the allocated memory for path table
    free(path_table);

    // Return the result
    return result;   
}

// Entry point
int alg_dp_longest_increasing_path_main(int argc, char* argv[])
{
    // Initialize the input matrix
    int n = 4;
    int ** A = (int **) malloc (n * sizeof(int *));
    for (int i = 0; i < n; ++i) A[i] = (int *) malloc (n * sizeof(int));
    A[0][0] =  7; A[0][1] =  6; A[0][2] =  5; A[0][3] = 4;
    A[1][0] =  8; A[1][1] = 15; A[1][2] = 14; A[1][3] = 3;
    A[2][0] =  9; A[2][1] = 15; A[2][2] = 13; A[2][3] = 2;
    A[3][0] = 10; A[3][1] = 11; A[3][2] = 12; A[3][3] = 1;

    // Compute the longest increasing path
    longest_increasing_path_result result = longest_increasing_path((const int **) A, n);

    // Print the result
    printf("\033[1;32m>>> Longest Increasing Path [RESULT] >>>\033[0m\n");
    printf("  ├── \033[1mInput Matrix:\033[0m\n");
    for (int i = 0; i < n; ++i)
    {
        if (i < n - 1) printf("  │   │   ");
        else printf("  │   └── ");
        for (int j = 0; j < n; ++j)
            printf("%4d ", A[i][j]);
        printf("\n");
    }
    printf("  ├── \033[1mLongest Increasing Path Length:\033[0m %d\n", result.length);
    printf("  ├── \033[1mLongest Increasing Path (cell coordinates):\033[0m ");
    for (int i = 0; i < result.length; ++i)
        printf("(%d, %d) ", result.path[i][0], result.path[i][1]);
    printf("\n");
    // Print the matrix with the longest increasing path highlighted
    printf("  └── \033[1mLongest Increasing Path Highlighted in Matrix:\033[0m\n");
    for (int i = 0; i < n; ++i)
    {
        if (i < n - 1) printf("      │   ");
        else printf("      └── ");
        for (int j = 0; j < n; ++j)
        {
            // Check if the current cell is part of the longest increasing path
            int is_in_path = 0;
            for (int k = 0; k < result.length; ++k)
            {
                if (result.path[k][0] == i && result.path[k][1] == j)
                {
                    is_in_path = 1;
                    break;
                }
            }
            if (is_in_path) printf("\033[1;31m%4d\033[0m ", A[i][j]); // Highlight the cell in red
            else printf("%4d ", A[i][j]);
        }
        printf("\n");
    }

    // Free the allocated memory for the input matrix
    for (int i = 0; i < n; ++i) free(A[i]);
    free(A);
    // Free the allocated memory for the longest increasing path
    free(result.path);

    return 0;
}