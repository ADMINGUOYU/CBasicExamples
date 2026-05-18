// Dynamic Programming over Intervals
// Longest Palindromic Substring (must be contiguous)

/*
Given a string X = x1x2...xn, find the longest palindromic substring.
Def: A palindrome is a string that reads the same backward or forward.

Example:
X: ACCABA
Palindromic substrings: A, C, C, A, B, A, CC, ACA, ABA, ACCA
Longest palindromic substring: ACCA
Output (longest): ACCA

IDEA:
1. Brute-force approach:
   We can generate all possible substrings of X and check if they are
   palindromic, keeping track of the longest one found. 
   This approach has a time complexity of O(n^3) in the worst case,
   where n is the length of X. (n^2 for generating substrings and n for
   checking if each substring is a palindrome - MAX length n)
2. Dynamic Programming approach:
   We can use a 2D boolean table P[i][j] to indicate whether the
   substring X[i..j] is a palindrome. We can fill this table using the
   following rules:
   - P[i][i] = true for all i (every single character is a palindrome)
   - P[i][i+1] = (X[i] == X[i+1]) for all i (two-character substrings
     are palindromes if both characters are the same)
   - For substrings of length greater than 2:
     P[i][j] = (X[i] == X[j]) && P[i+1][j-1]
   We can fill this table in a bottom-up manner, starting with substrings
   of length 1 and 2, and then moving to longer substrings. While filling
   the table, we can keep track of the longest palindromic substring found.
   This approach has a time complexity of O(n^2) and a space complexity of
   O(n^2) due to the 2D table.

INTUITION:
The diagonal of the table is like the leaves of a binary tree, and the
upper triangle is like the internal nodes.
We start from the leaves (single characters) and build up to the root
(the entire string). Each entry in the table depends on the entries below
it, which is why we fill it in a bottom-up manner. The longest palindromic
substring will be found as we fill the table, and we can keep track of its
length and starting index to return it at the end.
[Breaking the array till single element and then reconstruct, like the
binary sort - related ideas.]
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Result structure
typedef struct lps_result
{
    // Length of the longest palindromic substring
    int length;
    // Starting index of the LPS in the original string
    int start_index;
    // The longest palindromic substring (null-terminated)
    // len(lps_string) = length of the longest palindromic substring + 1 (for null terminator)
    char * lps_string;
} lps_result;

// Solver function
static lps_result longest_palindromic_substring(const char * X)
{
    // Get the length of the input string
    int n = strlen(X);

    // Initialize the DP table - init to false (0)
    // use smallest possible type
    char ** P = (char **) malloc (n * sizeof(char *));
    for (int i = 0; i < n; ++i) P[i] = (char *) malloc (n * sizeof(char));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            P[i][j] = 0;

    // Variables to keep track of the longest palindromic substring
    int max_length = 1; // at least every single character is a palindrome
    int start_index = 0; // starting index of the longest palindromic substring

    // Fill the DP table
    for (int length = 1; length <= n; ++length) // length of the substring
    {
        for (int i = 0; i <= n - length; ++i) // starting index of the substring
        {
            // This for-loop iterates [0, n-length],
            // n-length is the last starting index for a substring of given
            // length to fit in the string of length n

            // Get ending index of the substring
            int j = i + length - 1;

            // if length is 1, fill the table with true (1)
            if (length == 1)
            {
                P[i][j] = 1;    // Actually, i == j
                continue;
            }

            // if length is 2, another base case check
            else if (length == 2)
                P[i][j] = (X[i] == X[j]) ? 1 : 0;

            // for other cases, the truth value is based
            // on the inner substring and the two end characters
            else
                P[i][j] = (X[i] == X[j]) && P[i + 1][j - 1];

            // Update the longest palindromic substring if needed
            if (P[i][j] && (length > max_length))
            {
                max_length = length;
                start_index = i;
            }
        }
    }

    // [DEBUG] print the DP table
    printf("\033[1;34m>>> DP Table P[i][j] (1 if X[i..j] is a palindrome, 0 otherwise) >>>\033[0m\n");
    printf("%3c", ' ');
    for (int j = 0; j < n; ++j)
        printf("\033[1m%3c\033[0m", X[j]);
    printf("\n");
    for (int i = 0; i < n; ++i)
    {
        printf("\033[1m%3c\033[0m", X[i]);
        for (int j = 0; j < n; ++j)
        {
            if (j < i)
            {
                printf("%3c", ' '); // empty space for j < i, since we only fill upper triangle
                continue;
            }
            
            if (P[i][j])
                printf("\033[1;32m%3d\033[0m", P[i][j]);
            else
                printf("%3d", P[i][j]);
        }
        printf("\n");
    }

    // Prepare the longest palindromic substring result
    char * lps_string = (char *) malloc ((max_length + 1) * sizeof(char)); // +1 for null terminator
    strncpy(lps_string, X + start_index, max_length);
    lps_string[max_length] = '\0'; // null terminator

    // Prepare the result structure
    lps_result result;
    result.length = max_length;
    result.start_index = start_index;
    result.lps_string = lps_string;

    // Free the DP table memory
    for (int i = 0; i < n; ++i) free(P[i]);
    free(P);

    // Return the result
    return result;
}

// Entry point
int alg_dp_int_longest_palindromic_substring_main(int argc, char* argv[])
{
    // Prepare string input
    const char * X = "ACCABA";

    // Call the solver function
    lps_result result = longest_palindromic_substring(X);

    // Print the result
    printf("\033[1;32m>>> Longest Palindromic Substring [RESULT] >>>\033[0m\n");
    printf("  ├── \033[1mInput string X:\033[0m %s\n", X);
    printf("  ├── \033[1mLength of LPS:\033[0m %d\n", result.length);
    printf("  ├── \033[1mStarting index of LPS in X:\033[0m %d\n", result.start_index);
    printf("  ├── \033[1mLPS:\033[0m %s\n", result.lps_string);
    // Print the longest palindromic substring with indices highlighted
    printf("  └── \033[1mLPS in X:\033[0m ");
    for (int i = 0; X[i] != '\0'; ++i)
    {
        if (i >= result.start_index && i < result.start_index + result.length)
            printf("\033[1;32m%c\033[0m", X[i]);
        else
            printf("%c", X[i]);
    }
    printf("\n");

    // Free the allocated memory for lps_string
    free(result.lps_string);

    return 0;
}