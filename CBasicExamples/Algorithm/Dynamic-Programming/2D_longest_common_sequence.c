// 2D Dynamic Programming
// Longest Common Subsequence

/*
Given two sequences:
𝑋 = (𝑥1 , 𝑥2 , … , 𝑥𝑚 ) and 
𝑌 = (𝑦1 , 𝑦2 , … , 𝑦𝑛 ), 
we say that 𝑍 = 𝑧1 , 𝑧2 , … , 𝑧𝑘 is a common subsequence of 𝑋 and 𝑌 if
𝑥_𝑖_𝑝 = 𝑦_𝑗_𝑝 = 𝑧𝑝 for all 𝑝 = 1, 2, … , 𝑘
where 𝑖1 < 𝑖2 < ⋯ < 𝑖𝑘 and 𝑗1 < 𝑗2 < ⋯ < 𝑗𝑘

The goal is to find the longest common subsequence of 𝑋 and 𝑌.

Example:
𝑋: A B A C   B D A B
𝑌:   B D C A B   A
𝑍:   B   C   B   A

Observation: The problem is equivalent to finding the maximum
matching between 𝑋 and 𝑌 such that matched pairs don’t cross
(cross means that if 𝑥𝑖 is matched with 𝑦𝑗, then 𝑥𝑘 can be matched with 𝑦𝑙 only if 𝑖 < 𝑘 and 𝑗 < 𝑙).

Brute-force approach:
We can generate all possible subsequences of 𝑋 and check if they are also
subsequences of 𝑌, keeping track of the longest one found. This approach
has a time complexity of O(2^m * m * n) in the worst case, where m is the
length of 𝑋 and n is the length of 𝑌.

No cross pairs and optimal substructure:
(inspiration) Supposed we have a matching between some prefix of 𝑋 and
some prefix of 𝑌. If the last characters of these prefixes are the same,
then we can include this character in our longest common subsequence and
look for the longest common subsequence of the remaining prefixes.
If the last characters are different, then we can either exclude the last
character of 𝑋 or exclude the last character of 𝑌 and look for the longest
common subsequence in those cases.
This leads to a recursive formulation that can be solved using dynamic
programming.

The longest common subsequence of 𝑋 and 𝑌 can be expressed in:
LCS(𝑋, 𝑌) = 
    0, if m = 0 or n = 0
    LCS(𝑋[1..m-1], 𝑌[1..n-1]) + 1, if 𝑥𝑚 = 𝑦𝑛
    max(LCS(𝑋[1..m-1], 𝑌), LCS(𝑋, 𝑌[1..n-1])), if 𝑥𝑚 ≠ 𝑦𝑛

*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Result structure
typedef struct lcs_result
{
    // Length of the longest common subsequence
    int length;
    // The longest common subsequence string (null-terminated)
    char * lcs_string;
} lcs_result;

// 2D DP solution for longest common subsequence problem
static lcs_result longest_common_subsequence(const char * X, const char * Y)
{
    /*
    Argument explanation:
    X : first input string (null-terminated)
    Y : second input string (null-terminated)
    */
    
    // Count the lengths of the input strings
    int X_len = 0; while (X[X_len] != '\0') ++X_len;
    int Y_len = 0; while (Y[Y_len] != '\0') ++Y_len;

    // Initialize the DP table
    int ** L = (int **) malloc ((X_len + 1) * sizeof(int *));
    for (int i = 0; i <= X_len; ++i) L[i] = (int *) malloc ((Y_len + 1) * sizeof(int));
    for (int i = 0; i <= X_len; ++i)
        for (int j = 0; j <= Y_len; ++j)
            L[i][j] = 0;

    // Initialize Backtracking table ('↑, '←', '↖' for backtracking)
    // We use 'l' for left, 'u' for up, and 'd' for diagonal (match)
    char ** B = (char **) malloc ((X_len + 1) * sizeof(char *));
    for (int i = 0; i <= X_len; ++i) B[i] = (char *) malloc ((Y_len + 1) * sizeof(char));
    for (int i = 0; i <= X_len; ++i)
        for (int j = 0; j <= Y_len; ++j)
            B[i][j] = 0;

    // Fill the DP table
    for (int x_pos = 1; x_pos <= X_len; ++x_pos)
        for (int y_pos = 1; y_pos <= Y_len; ++y_pos)
        {
            // If the characters match, 
            // we can extend the longest common subsequence
            // (NOTE: -1 to switch to 0-based index for string access)
            if (X[x_pos - 1] == Y[y_pos - 1])
            {
                L[x_pos][y_pos] = L[x_pos - 1][y_pos - 1] + 1;
                B[x_pos][y_pos] = 'd'; // diagonal (match)
            }
            else
            {
                // If the characters don't match,
                // we take the maximum of the two possibilities
                // If EQUAL we prefer the one from the UP direction
                // (arbitrary choice)
                if (L[x_pos - 1][y_pos] >= L[x_pos][y_pos - 1])
                {
                    // Take the value from the cell above
                    L[x_pos][y_pos] = L[x_pos - 1][y_pos];
                    B[x_pos][y_pos] = 'u'; // up
                }
                else
                {
                    // Take the value from the cell to the left
                    L[x_pos][y_pos] = L[x_pos][y_pos - 1];
                    B[x_pos][y_pos] = 'l'; // left
                }
            }
        }
    
    // Backtrack to find the longest common subsequence string
    int lcs_length = L[X_len][Y_len];
    char * lcs_string = (char *) malloc ((lcs_length + 1) * sizeof(char)); // +1 for null terminator
    lcs_string[lcs_length] = '\0'; // null terminator
    int x_pos = X_len, y_pos = Y_len, lcs_pos = lcs_length - 1;
    while (x_pos > 0 && y_pos > 0)
    {
        if (B[x_pos][y_pos] == 'd')
        {
            // This character is part of the longest common subsequence
            lcs_string[lcs_pos] = X[x_pos - 1]; // or Y[y_pos - 1], they are the same
            --lcs_pos;
            --x_pos;
            --y_pos;
        }
        else if (B[x_pos][y_pos] == 'u')
            --x_pos; // move up
        else
            --y_pos; // move left
    }

    // [DEBUG] print the DP table
    printf("\033[1;34m>>> DP Table (L) >>>\033[0m (including row 0 and column 0)\n");
    for (int i = 0; i <= X_len; ++i)
    {
        for (int j = 0; j <= Y_len; ++j)
            printf("%4d ", L[i][j]);
        printf("\n");
    }
    printf("\n");
    // [DEBUG] print the Backtracking table
    printf("\033[1;34m>>> Backtracking Table (B) >>>\033[0m (including row 0 and column 0)\n");
    for (int i = 0; i <= X_len; ++i)
    {
        for (int j = 0; j <= Y_len; ++j)
            printf("%4c ", B[i][j] == 0 ? '-' : B[i][j]);
        printf("\n");
    }
    printf("\n");

    // Prepare the result
    lcs_result result;
    result.length = lcs_length;
    result.lcs_string = lcs_string;

    // Free the allocated memory for DP and Backtracking tables
    for (int i = 0; i <= X_len; ++i) free(L[i]);
    free(L);
    for (int i = 0; i <= X_len; ++i) free(B[i]);
    free(B);

    // Return the result
    return result;
}

// Entry point
int alg_dp_2d_longest_common_sequence_main(int argc, char* argv[])
{
    // Initialize input strings
    const char * X = "ABACBDAB";
    const char * Y = "BDCABA";
    // 𝑍 : B C B A is a solution
    // 𝑍’: B A B A is another legal solution

    // Compute the longest common subsequence
    lcs_result result = longest_common_subsequence(X, Y);

    // Print the result
    printf("\033[1;32m>>> Longest Common Subsequence [RESULT] >>>\033[0m\n");
    printf("  ├── \033[1mInput string X:\033[0m %s\n", X);
    printf("  ├── \033[1mInput string Y:\033[0m %s\n", Y);
    printf("  ├── \033[1mLength of longest common subsequence:\033[0m %d\n", result.length);
    printf("  └── \033[1mLongest common subsequence string:\033[0m %s\n", result.lcs_string);

    // Free the allocated memory for lcs_string
    free(result.lcs_string);

    return 0;
}