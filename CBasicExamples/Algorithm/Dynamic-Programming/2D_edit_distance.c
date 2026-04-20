// 2D Dynamic Programming
// Edit Distance

/*
Given two strings 𝑠 and 𝑡, the edit distance 𝑒𝑑𝑖𝑡(𝑠, 𝑡) is the smallest
number of following edit operations to **turn 𝑠 into 𝑡**:
 - Insertion:    add a letter
 - Deletion:     remove a letter
 - Substitution: replace a character with another one.

Example:
𝑠 = abode and 𝑡 = blog.
Then, 𝑒𝑑𝑖𝑡(𝑠, 𝑡) = 4 operations
Start from abode
 1) delete a ⇒ bode
 2) insert l after b ⇒ blode
 3) delete d ⇒ bloe
 4) substitute e with g ⇒ blog
Impossible to do so with at most 3 operations.

Intuition:
 1) You have to add some letters in 𝑠 to get 𝑡,
    so you need at least |𝑡| - |𝑠| insertions. (if too few of characters)
 2) You have to delete some letters in 𝑠 to get 𝑡,
    so you need at least |𝑠| - |𝑡| deletions. (if too many characters)
 3) You have to substitute some letters in 𝑠 to get 𝑡,
    so you need at least the number of differing characters.

Recurrence:
Let 𝑠 and 𝑡 be two strings with lengths 𝑚 and 𝑛, respectively.
Case 1 If 𝑚 = 0, then 𝑒𝑑𝑖𝑡(𝑠, 𝑡) = 𝑛.
Case 2 If 𝑛 = 0, then 𝑒𝑑𝑖𝑡(𝑠, 𝑡) = 𝑚.
Case 3 If 𝑚 > 0, 𝑛 > 0, and 𝑠[𝑚] = 𝑡[𝑛], then
    𝑒𝑑𝑖𝑡(𝑠, 𝑡) = min(
                    1 + 𝑒𝑑𝑖𝑡(𝑠[1 .. 𝑚],     𝑡[1 .. 𝑛 - 1]), 
                    1 + 𝑒𝑑𝑖𝑡(𝑠[1 .. 𝑚 - 1], 𝑡[1 .. 𝑛]), 
                        𝑒𝑑𝑖𝑡(𝑠[1 .. 𝑚 - 1], 𝑡[1 .. 𝑛 - 1])
                  )
    * Delete 𝑡[𝑛], and use the least number of edit operations
    to change 𝑠[1 .. 𝑚] into 𝑡[1 .. 𝑛 − 1]. The total number of
    edit operations is therefore 1 + 𝑒𝑑𝑖𝑡(𝑠[1 .. 𝑚] , 𝑡[1 .. 𝑛 − 1]).
    (example: 𝑠 = 𝑎𝑏𝑐, 𝑡 = 𝑎𝑏𝑐𝑐)
    * Delete 𝑠[𝑚], and use the least number of edit operations
    to change 𝑠[1 .. 𝑚 − 1] into 𝑡[1 .. 𝑛]. The total number of
    edit operations is therefore 1 + 𝑒𝑑𝑖𝑡(𝑠[1 .. 𝑚 − 1] , 𝑡[1 .. 𝑛]).
    (example: 𝑠 = 𝑎𝑏𝑐𝑐, 𝑡 = 𝑎𝑏𝑐)
    * Simply change 𝑠[1 .. 𝑚 − 1] into 𝑡[1 .. 𝑛 − 1]. The total
    number of edit operations is therefore
    𝑒𝑑𝑖𝑡(𝑠[1 .. 𝑚 − 1] , 𝑡[1 .. 𝑛 − 1]).
    (example: 𝑠 = 𝑎𝑏𝑐𝑐, 𝑡 = 𝑎𝑏𝑐𝑐)
Case 4 If 𝑚 > 0, 𝑛 > 0, and 𝑠[𝑚] ≠ 𝑡[𝑛], then
    𝑒𝑑𝑖𝑡(𝑠, 𝑡) = min(
                    1 + 𝑒𝑑𝑖𝑡(𝑠[1 .. 𝑚],     𝑡[1 .. 𝑛 - 1]), 
                    1 + 𝑒𝑑𝑖𝑡(𝑠[1 .. 𝑚 - 1], 𝑡[1 .. 𝑛]), 
                    1 + 𝑒𝑑𝑖𝑡(𝑠[1 .. 𝑚 - 1], 𝑡[1 .. 𝑛 - 1])
                  )

Lets store 𝑒𝑑𝑖𝑡(𝑖, 𝑗) in an array 𝐸[𝑖, 𝑗]. Then
𝐸[𝑖, 𝑗] = min(
                1 + 𝐸[𝑖, 𝑗 - 1], -> MAPPED to INSERTION
                1 + 𝐸[𝑖 - 1, 𝑗], -> MAPPED to DELETION
                (𝑠[𝑖] == 𝑡[𝑗] ? 0 : 1) + 𝐸[𝑖 - 1, 𝑗 - 1] -> MAPPED to SUBSTITUTION (if characters are different)
              )

DP Algorithm for filling array 𝐸 (fill 𝐸[𝑖, 𝑗] from top-left to bottom-right)
 1) Fill in row 0 and column 0.
 2) Fill in the cells of row 1 from left to right.
 3) Fill in the cells of row 2 from left to right.
 4) ...
 5) Fill in the cells of row 𝑚 from left to right.

Time complexity: O(𝑚𝑛)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Enumeration for edit operations
typedef enum edit_operation
{
    INSERTION    = (char)0,
    DELETION     = (char)1,
    SUBSTITUTION = (char)2,
    VOID         = (char)3 // No operation (used for base cases)
} edit_operation;

// Edit structure
typedef struct edit
{
    // The edit operation
    edit_operation op;
    // The position of the edit in the string
    int position;
    // The character involved in the edit
    // For insertion: the character to be inserted
    // For deletion: the character to be deleted
    // For substitution: the character to be substituted
    char character;
} edit;

// Result structure to hold the edit distance and the operations
typedef struct edit_distance_result
{
    // The list of edit operations to transform s into t
    edit *operations;
    // The number of edit operations
    int num_operations;
} edit_distance_result;

// Function to calculate the edit distance and the operations
edit_distance_result calculate_edit_distance(const char *s, const char *t)
{
    // Get the lengths of the input strings
    int m = strlen(s);
    int n = strlen(t);

    // Create a 2D array to store the edit distances
    int **E = (int **)malloc((m + 1) * sizeof(int *));
    for (int i = 0; i <= m; ++i)
    {
        // Allocate memory for each row of the 2D array
        E[i] = (int *)malloc((n + 1) * sizeof(int));
        // Base case: If s is empty, the edit distance is the length of t (all insertions)
        E[i][0] = i;
    }
    // Base case: If t is empty, the edit distance is the length of s (all deletions)
    for (int j = 0; j <= n; ++j)
        E[0][j] = j;

    // Create a 2D array to store the edit operations
    edit **operations = (edit **)malloc((m + 1) * sizeof(edit *));
    for (int i = 0; i <= m; ++i)
        operations[i] = (edit *)malloc((n + 1) * sizeof(edit));
    // Initialize the operations for the base cases
    operations[0][0].op = VOID; // No operation needed for transforming empty string to empty string
    operations[0][0].position = 0; // Position is not relevant for the base case
    operations[0][0].character = '\0'; // No character involved in the base case
    // For the first row (s is empty), all operations are insertions
    for (int j = 1; j <= n; ++j)
    {
        operations[0][j].op = INSERTION;
        operations[0][j].position = 0; // Position is not relevant for insertions in the base case
        operations[0][j].character = t[j - 1]; // Character to be inserted
    }
    // For the first column (t is empty), all operations are deletions
    for (int i = 1; i <= m; ++i)
    {
        operations[i][0].op = DELETION;
        operations[i][0].position = i; // Position of the character to be deleted
        operations[i][0].character = s[i - 1]; // Character to be deleted
    }

    // DP iteration
    for (int source = 1; source <= m; ++source)
    {
        for (int target = 1; target <= n; ++target)
        {
            // If the characters are the same, no operation is needed
            // NOTE: source and target are 1-based indices for the DP table,
            //       so we need to access s[source - 1] and t[target - 1]
            if (s[source - 1] == t[target - 1])
            {
                // No operation needed, characters are the same
                E[source][target] = E[source - 1][target - 1];
                operations[source][target].op = VOID;
            }
            else
            {
                // Calculate the cost of insertion, deletion, and substitution
                int insertion_cost = E[source][target - 1] + 1;
                int deletion_cost = E[source - 1][target] + 1;
                int substitution_cost = E[source - 1][target - 1] + 1;

                // Find the minimum cost and corresponding operation
                if (insertion_cost <= deletion_cost && insertion_cost <= substitution_cost)
                {
                    E[source][target] = insertion_cost;
                    operations[source][target].op = INSERTION;
                    operations[source][target].position = source; // Position in s where insertion occurs
                    operations[source][target].character = t[target - 1]; // Character to be inserted
                }
                else if (deletion_cost <= insertion_cost && deletion_cost <= substitution_cost)
                {
                    E[source][target] = deletion_cost;
                    operations[source][target].op = DELETION;
                    operations[source][target].position = source; // Position in s where deletion occurs
                    operations[source][target].character = s[source - 1]; // Character to be deleted
                }
                else
                {
                    E[source][target] = substitution_cost;
                    operations[source][target].op = SUBSTITUTION;
                    operations[source][target].position = source; // Position in s where substitution occurs
                    operations[source][target].character = t[target - 1]; // Character to substitute with
                }
            }
        }
    }

    // [DEBUG] Print the edit distance table
    printf("\033[1;34m>>> Edit Distance Table (E) >>>\033[0m (including row 0 and column 0)\n");
    // print first row (header for target string)
    printf("%4c ", ' '); // empty cell for the top-left corner
    for (int j = 0; j <= n; ++j)
        if (j == 0)
            printf("%4s ", "tgt>");
        else
            printf("\033[1m%4c\033[0m ", t[j - 1]);
    printf("\n");
    for (int i = 0; i <= m; ++i)
    {
        // print first column (header for source string)
        if (i == 0)
            printf("%4c ", 0x2d); // empty cell col 0
        else
            printf("\033[1m%4c\033[0m ", s[i - 1]);
        for (int j = 0; j <= n; ++j)
            printf("%4d ", E[i][j]);
        printf("\n");
    }
    printf("\n");
    // [DEBUG] Print the operations table
    printf("\033[1;34m>>> Edit Operations Table >>>\033[0m (including row 0 and column 0)\n");
    // print first row (header for target string)
    printf("%4c ", ' '); // empty cell for the top-left corner
    for (int j = 0; j <= n; ++j)
        if (j == 0)
            printf("%4s ", "tgt>");
        else
            printf("\033[1m%4c\033[0m ", t[j - 1]);
    printf("\n");
    for (int i = 0; i <= m; ++i)
    {
        // print first column (header for source string)
        if (i == 0)
            printf("%4c ", 0x2d); // empty cell col 0
        else
            printf("\033[1m%4c\033[0m ", s[i - 1]);
        for (int j = 0; j <= n; ++j)
        {
            edit current_op = operations[i][j];
            if (current_op.op == INSERTION)
                printf("\033[1;32m%4s\033[0m ", "INS");
            else if (current_op.op == DELETION)
                printf("\033[1;31m%4s\033[0m ", "DEL");
            else if (current_op.op == SUBSTITUTION)
                printf("\033[1;33m%4s\033[0m ", "SUB");
            else
                printf("\033[1;36m%4s\033[0m ", "VOID");
        }
        printf("\n");
    }
    printf("\n");

    // The edit distance is now stored in E[m][n]
    int edit_distance = E[m][n];

    // Backtrack to find the sequence of edit operations
    edit *edit_operations = (edit *)malloc(edit_distance * sizeof(edit));
    int index = edit_distance - 1; // Start from the end of the operations array
    int source = m;
    int target = n;
    while (source > 0 || target > 0)
    {
        edit current_op = operations[source][target];
        if (current_op.op == VOID)
        {
            // No operation, move diagonally
            source--;
            target--;
        }
        else
        {
            // Store the current operation in the edit_operations array
            // Also, decrease the index for the next operation
            edit_operations[index--] = current_op;

            // Move according to the type of operation
            if (current_op.op == INSERTION)
                target--; // Move left for insertion
            else if (current_op.op == DELETION)
                source--; // Move up for deletion
            else if (current_op.op == SUBSTITUTION)
            {
                source--; // Move up for substitution
                target--; // Move left for substitution
            }
        }
    }

    // Free the allocated memory for the DP table and operations
    for (int i = 0; i <= m; ++i)
    {
        free(E[i]);
        free(operations[i]);
    }
    free(E);
    free(operations);

    // Return the result
    return (edit_distance_result){edit_operations, edit_distance};
}

// Entry point
int alg_dp_2d_edit_distance_main(int argc, char* argv[])
{
    // Initialize two strings for testing
    const char *s = "abode";
    const char *t = "blog";

    // Calculate the edit distance and the operations
    edit_distance_result result = calculate_edit_distance(s, t);

    // Print the edit distance
    printf("\033[1;32m>>> Edit Distance [RESULT] >>>\033[0m\n");
    printf("  ├── \033[1mString source[s]:\033[0m %s\n", s);
    printf("  ├── \033[1mString target[t]:\033[0m %s\n", t);
    printf("  ├── \033[1mEdit Distance:\033[0m %d\n", result.num_operations);
    printf("  └── \033[1mEdit Operations:\033[0m\n");
    for (int i = 0; i < result.num_operations; ++i)
    {
        if (i < result.num_operations - 1) printf("      ├── ");
        else printf("      └── ");

        // Get the current edit operation
        // NOTE: the position might NOT be correct after insertions/deletions
        edit current_op = result.operations[i];
        if (current_op.op == INSERTION)
            printf("Insert '%c' at position %d\n", current_op.character, current_op.position);
        else if (current_op.op == DELETION)
            printf("Delete '%c' from position %d\n", current_op.character, current_op.position);
        else if (current_op.op == SUBSTITUTION)
            printf("Substitute '%c' with '%c' at position %d\n", s[current_op.position - 1], current_op.character, current_op.position);
    }

    // Free the allocated memory for edit operations
    free(result.operations);

    return 0;
}