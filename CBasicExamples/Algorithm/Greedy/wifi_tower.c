// Greedy Algorithm
// WIFI Tower Problem

/*
You are given the locations of n buildings B1, B2, ..., Bn on the
real line. Each location is a coordinate (i.e. a real number).
Each building Bi has a WIFI signal receiver with a range of ri > 0.
That is, if we place a signal tower at distance ri or less from Bi,
then Bi gets WIFI. 
Note that the signal receivers of different buildings may have
different ranges.

Describe a greedy algorithm that places the smallest number of signal
towers so that every building gets WIFI. Prove the correctness of your
algorithm. Derive the running time of your algorithm.

IDEA:
 - We sort the buildings on their (locations + range) in increasing order
   (i.e. the rightmost point that the building can be covered)
 - we place a station at the very last moment where the current building
   can be covered (i.e. at location + range)
 - We go to the first building that the just placed antenna cannot cover
 - REPEAT till end

PROOF:
 - Assume optimum solution with antenna location (increasing order):
   [O]: o1, o1, o3, o4, ... 
 - Assume the greedy solution with antenna location (increasing order):
   [G]: g1, g2, g3, g4, ...
 - Let g1 be the first difference between the two, we know for sure that
   o1 and g1 must cover B1 (sorted minimum coordinate), and o1 < g1 (how
   we defined the greedy algorithm)
 - SWAP g1 -> o1 get O'
 - O' covers B1, for B2 if previously covered by o1, then definitely covered
   by g1, if previously covered by o2, then no change, still covered
   -> proves that O' is a valid AND also optimum solution
 - We repeat the swapping process to construct O', eventually O' == G ->
   This proves that our greedy solution is optimum

RUNNING TIME:
 - Sorting the buildings: O(n log n) -> we use qsort in C in this implementation
 - Placing the towers: O(n) -> we iterate through the buildings once
 - Overall: O(n log n) + O(n) = O(n log n)
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Building structure
typedef struct building
{
    double location;
    double range;
} building;

// WIFI tower result structure
typedef struct wifi_tower_result
{
    // Number of WIFI towers placed
    int tower_count;
    // Locations of WIFI towers placed
    // e.g. towers_placed[0] = 5.0 means the first tower placed is at location 5.0
    // NOTE: buffer size is n (number of buildings), but only the first tower_count entries are valid
    //       we use 0.0 to indicate invalid entry
    double * towers_placed;
} wifi_tower_result;

// Helper function to compare buildings by location (for qsort)
static int compare_buildings(const void * a, const void * b)
{
    building * building_a = (building *) a;
    building * building_b = (building *) b;

    // We use > / < for stable floating point comparison

    // Get location + range
    double sorting_value_a = building_a->location + building_a->range;
    double sorting_value_b = building_b->location + building_b->range;

    // Compare
    if (sorting_value_a < sorting_value_b) return -1;
    else if (sorting_value_a > sorting_value_b) return 1;
    else return 0;
}

// WIFI tower function
static wifi_tower_result wifi_tower(const building * buildings_ref, int n)
{
    // We first make a copy of the building array
    building * buildings = (building *) malloc (n * sizeof(building));
    for (int i = 0; i < n; ++i) buildings[i] = buildings_ref[i];

    // We sort based on the sorted values (location + range)
    // NOTE: qsort is in-place, so it modifies the buildings array
    qsort(buildings, n, sizeof(building), compare_buildings);
    
    // We initialize the result
    wifi_tower_result result;
    result.tower_count = 0;
    result.towers_placed = (double *) malloc (n * sizeof(double));

    // We iterate through the buildings and place the towers
    // i goes from 0 to n-1 -> running time O(n) (inside each iteration we do O(1) work)
    int i = 0;
    while (i < n)
    {
        // We place a tower at the last moment where the current building can be covered
        double tower_location = buildings[i].location + buildings[i].range;
        result.towers_placed[result.tower_count++] = tower_location;

        // We go to the first building that the just placed antenna cannot cover
        while (i < n && buildings[i].location - buildings[i].range <= tower_location) ++i;
    }

    // Free the allocated memory for the buildings copy
    free(buildings);

    return result;
}

// Entry point
int alg_greedy_wifi_tower_main(int argc, char* argv[])
{
    // Initialize the building and other parameters
    building buildings[] = {
        // {location, range}
        { 1.0, 2.0 },
        { 2.5, 0.5 },
        { 3.0, 4.0 },
        { 6.0, 1.0 },
        { 7.0, 0.2 },
        { 8.5, 3.0 },
        { 12.0, 1.0 },
        { 15.0, 0.5 },
    };

    // Get length of buildings array
    int n = sizeof(buildings) / sizeof(buildings[0]);

    // Get the result
    wifi_tower_result result = wifi_tower(buildings, n);

    // Print the result
    printf("\033[1;32m>>> WIFI Tower Problem [RESULT] >>>\033[0m\n");
    printf("  ├── \033[1mInput size (n):\033[0m %d\n", n);
    printf("  ├── \033[1mInput Buildings:\033[0m\n");
    for (int i = 0; i < n; ++i)
    {
        if (i < n - 1) printf("  │   ├── ");
        else printf("  │   └── ");
        printf("Building %d: location = %.2f, range = %.2f\n", i + 1, buildings[i].location, buildings[i].range);
    }
    printf("  ├── \033[1mNumber of WIFI towers placed:\033[0m %d\n", result.tower_count);
    printf("  └── \033[1mLocations of WIFI towers placed:\033[0m\n");
    for (int i = 0; i < result.tower_count; ++i)
    {
        if (i < result.tower_count - 1) printf("      ├── ");
        else printf("      └── ");
        printf("Tower %d: location = %.2f\n", i + 1, result.towers_placed[i]);
    }

    // Free the allocated memory for towers_placed
    free(result.towers_placed);

    return 0;
}