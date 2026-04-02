// Dynamic Programming
// Highway Billboards Problem

/*
Consider a highway from west to east. The possible sites for billboards
are given by numbers x_1, x_2, ..., x_n, each in the interval [0, L].
If you place a billboard at location x_i, you receive revenue of r_i > 0.

Regulations imposed by the county's Highway Department require that every
two of the billboards must be at least d miles apart. 
You wish to place billboards at a subset of sites so as to maximize your
total revenue, subject to this restriction. 

Design an efficient algorithm for this problem. 

NOTE: brute force: O(2^n) time complexity (each site has 2 choices: place or not place), 
                   O(n) space complexity
      dynamic programming: 
                   O(n) time complexity, 
                   O(n) space complexity
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Billboard structure
typedef struct billboard
{
    int location;
    int revenue;
} billboard;

// Highway billboards result structure
typedef struct highway_billboard_result
{
    // Maximum revenue
    int max_revenue;
    // Billboards placed to achieve maximum revenue
    // Records locations of billboards placed
    // e.g. billboards_placed[0] = 5 means the first billboard
    // placed is at location 5
    // NOTE: buffer size is n (number of possible sites), 
    //       but only the first billboard_count entries are valid
    //       we use 0 to indicate invalid entry
    int * billboards_placed;
} highway_billboard_result;

// Helper function to compare billboards by location (for qsort)
int compare_billboards(const void * a, const void * b)
{
    billboard * billboard_a = (billboard *) a;
    billboard * billboard_b = (billboard *) b;
    return billboard_a->location - billboard_b->location;
}

// Highway billboards function
highway_billboard_result highway_billboards(const billboard * billboards_ref, int n, int L, int d)
{
    /*
    Argument explanation:
    billboards_ref
       : array of possible billboards (location and revenue)
       NOTE: we have to sort billboards_ref by location in ascending order later
    n  : len(billboards_ref)
    L  : length of highway
    d  : minimum distance between any two billboards
    */

    // Copy billboards_ref to a new array so that we can sort it
    billboard * billboards = (billboard *) malloc((n + 1) * sizeof(billboard));
    for (int i = 1; i <= n; ++i) billboards[i] = billboards_ref[i - 1];
    // Sort billboards by location
    qsort(billboards, n, sizeof(billboard), compare_billboards);
    // now, billboards[1] is the billboard with the smallest location,

    // We build the latest compatible array
    // NOTE: LATEST_COMPATIBLE[i] is the last available spot considering the 'd' constrain
    int * LATEST_COMPATIBLE = (int * ) malloc ((n + 1) * sizeof(int));
    for (int i = 0; i <= n; ++i) LATEST_COMPATIBLE[i] = 0;
    // Use binary search to find the latest compatible idx
    for (int i = 1; i <= n; ++i)
    {
        // search in spot [1 ... n - 1]
        int left = 1;
        int right = n - 1;
        int latest_compatible_location_idx = 0; // defaults to 0
        // loop search
        while (left <= right)
        {
            int mid = left + (right - left) / 2;
            if (billboards[mid].location < (billboards[i].location - d))
            {
                // mid is compatible -> we search right half
                latest_compatible_location_idx = mid;
                left = mid + 1;
            }
            else
            {
                // mid is not compatible -> we search left half
                right = mid - 1;
            }
            LATEST_COMPATIBLE[i] = latest_compatible_location_idx;
        }
    }

    // Initialize MAX_REVENUE array
    int * MAX_REVENUE = (int *) malloc ((n + 1) * sizeof(int));
    MAX_REVENUE[0] = 0;
    for (int i = 1; i <= n; ++i) MAX_REVENUE[i] = INT_MIN;

    // Initialize BACK_TRACK array
    // NOTE: BACK_TRACK[i] == 0 -> not place at location i
    int * BACK_TRACK = (int *) malloc ((n + 1) * sizeof(int));
    for (int i = 0; i <= n; ++i) BACK_TRACK[i] = 0;

    // main loop
    int last_billboard_idx = 1;
    for ( ; last_billboard_idx <= n; ++last_billboard_idx)
    {
        // consider first [ 1 ... i ] locations
        int billboard_location = billboards[last_billboard_idx].location;
        // Considering road length L, we stop if exceeded
        if (billboard_location > L) break;
        int billboard_last_compatible_idx = LATEST_COMPATIBLE[last_billboard_idx];
        
        // calculate revenue
        int select_this_revenue = billboards[last_billboard_idx].revenue + MAX_REVENUE[billboard_last_compatible_idx];

        // Decide which to use
        if (MAX_REVENUE[last_billboard_idx - 1] < select_this_revenue)
        {
            // we select this
            BACK_TRACK[last_billboard_idx] = 1;
            // record this revenue
            MAX_REVENUE[last_billboard_idx] = select_this_revenue;
        }
        else
        {
            // we don't select this
            MAX_REVENUE[last_billboard_idx] = MAX_REVENUE[last_billboard_idx - 1];
        }
    }

    // Decrease last_billboard_idx by 1 to get the last idx we considered
    --last_billboard_idx;

    // Depend on the last billboard idx we considered, we prepare the result
    highway_billboard_result result;
    result.max_revenue = MAX_REVENUE[last_billboard_idx];
    result.billboards_placed = (int *) malloc (n * sizeof(int));
    for (int i = 0; i < n; ++i) result.billboards_placed[i] = 0;
    // Trace back to get the billboards placed
    int billboard_count = 0;
    int current_billboard_idx = last_billboard_idx;
    while (current_billboard_idx > 0)
    {
        if (BACK_TRACK[current_billboard_idx] == 1)
        {
            // This billboard is placed
            result.billboards_placed[billboard_count] = billboards[current_billboard_idx].location;
            ++billboard_count;
            // Move to the latest compatible billboard
            current_billboard_idx = LATEST_COMPATIBLE[current_billboard_idx];
        }
        else
        {
            // Move to the previous billboard
            --current_billboard_idx;
        }
    }

    // Free the allocated memory for intermediate arrays
    free(billboards);
    free(LATEST_COMPATIBLE);
    free(MAX_REVENUE);
    free(BACK_TRACK);

    return result;    
}

// Entry point
int alg_dp_highway_billboards_main(int argc, char* argv[])
{
    // Initialize the billboard and other parameters
    billboard billboards[] = {
        // {location, revenue}
        {  6, 5 },
        {  9, 6 },
        { 12, 3 },
        { 14, 7 }
    };
    int n = sizeof(billboards) / sizeof(billboards[0]);
    int L = 15; // length of highway
    int d = 2;  // minimum distance between billboards

    // Get the result
    highway_billboard_result result = highway_billboards(billboards, n, L, d);

    // Print the result
    printf("\033[1;32m>>> Highway Billboards Problem [RESULT] >>>\033[0m\n");
    printf("  ├── \033[1mHighway length (L):\033[0m %d\n", L);
    printf("  ├── \033[1mMinimum distance (d):\033[0m %d\n", d);
    printf("  ├── \033[1mBillboard locations and revenues:\033[0m\n");
    for (int i = 0; i < n; ++i)
    {
        if (i < n - 1) printf("  │   ├── ");
        else printf("  │   └── ");
        printf("Location: %02d, Revenue: %02d\n", billboards[i].location, billboards[i].revenue);
    }
    printf("  ├── \033[1mMaximum revenue:\033[0m %d\n", result.max_revenue);
    printf("  └── \033[1mBillboards placed at locations:\033[0m ");
    for (int i = 0; i < n; ++i)
    {
        if (result.billboards_placed[i] == 0) break;
        printf("%02d ", result.billboards_placed[i]);
    }
    printf("\n");

    // Free the allocated memory
    free(result.billboards_placed);
    
    return 0;
}