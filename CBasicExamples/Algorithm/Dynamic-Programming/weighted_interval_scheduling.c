// Dynamic-Programming
// Weighted Interval Scheduling

/*
Given a set of n jobs where each job i has a start time s_{i},
a finish time f_{i} and a weight w_{i},
find a subset of non-overlapping jobs with maximum total weight.
NOTE: brute force: O(2^n) time complexity (each job has 2 choices), 
                   O(n) space complexity
      dynamic programming: 
                   O(n log n) time complexity (sorting + binary search),
                   O(n) space complexity
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Job structure
typedef struct job
{
    int job_id; // job index (for reference)
    int start_time;
    int finish_time;
    int weight;
} job;

// Weighted Interval Scheduling result structure
typedef struct wis_result
{
    // Maximum total weight of non-overlapping jobs
    int max_weight;
    // Jobs included in the optimal solution
    // Records indices of jobs included in the optimal solution
    // e.g. jobs_included[0] = 2 means the first job included in the optimal solution is job with index 2
    // NOTE: buffer size: total number of jobs
    // NOTE: index starts from 1 (job idx), if job_included[i] = 0, it means no more jobs included in the optimal solution
    int * jobs_included;
} wis_result;

// Compare function for qsort (sort by finish time)
static int compare(const void * a, const void * b)
{   
    /*
    Note regarding the return value:
    - < 0: If the first element should come before the second.
    - 0: If the elements are considered equal.
    - > 0: If the first element should come after the second. 
    */

    // Convert pointers to job pointers
    job * job_a = (job *)a;
    job * job_b = (job *)b;
    // return negative if job_a should come before job_b, 
    // positive if job_a should come after job_b, and 0 if they are equal
    return job_a->finish_time - job_b->finish_time;
}

// Weighted Interval Scheduling function
static wis_result weighted_interval_scheduling(const job * jobs_ref, int n)
{
    // Make a deep copy of jobs_ref
    job * jobs = (job * ) malloc ((n + 1) * sizeof(job));
    for (int i = 1; i <= n; ++i)
    {
        jobs[i].job_id = jobs_ref[i - 1].job_id;
        jobs[i].start_time = jobs_ref[i - 1].start_time;
        jobs[i].finish_time = jobs_ref[i - 1].finish_time;
        jobs[i].weight = jobs_ref[i - 1].weight;
    }

    // We have to sort the jobs by their finish times
    // NOTE: we can use any sorting algorithm, here we use qsort from stdlib
    // NOTE: qsort is an in-place sorting algorithm, it will modify the original array
    qsort(jobs, n, sizeof(job), compare);

    // NOTE: the reason we sort by finish time is that we want to
    // ensure that when we are processing job i, all jobs that finish
    // before job i are already processed -> we're building up the
    // solution by expanding the finish time of the jobs (interval)

    // we build up latest compatible array
    // NOTE: LATEST_COMPATIBLE[i] == the latest job that compatible with i
    int * LATEST_COMPATIBLE = (int * ) malloc ((n + 1) * sizeof(int));
    for (int i = 0; i <= n; ++i) LATEST_COMPATIBLE[i] = 0;
    // Use binary search to construct every element
    for (int i = 1; i <= n; ++i)
    {
        // search in jobs[1 ... n - 1]
        int left = 1;
        int right = n - 1;
        int latest_compatible_job_idx = 0; // default to 0 if no compatible job found
        while (left <= right)
        {
            int mid = left + (right - left) / 2;
            if (jobs[mid].finish_time <= jobs[i].start_time)
            {
                // mid is compatible, search in the right half to find a later compatible job
                latest_compatible_job_idx = mid; // update latest compatible job index
                left = mid + 1;
            }
            else
            {
                // mid is not compatible, search in the left half
                right = mid - 1;
            }
        }
        LATEST_COMPATIBLE[i] = latest_compatible_job_idx;
    }

    // Initialize maximum weight array
    // NOTE: MAXIMUM_WEIGHT[i] -> maximum weight of non-overlapping jobs among jobs[1 ... i]
    int * MAXIMUM_WEIGHT = (int * ) malloc ((n + 1) * sizeof(int));
    MAXIMUM_WEIGHT[0] = 0;
    for (int i = 1; i <= n; ++i) MAXIMUM_WEIGHT[i] = INT_MIN;

    // Initialize back trace array
    char * BACK_TRACE = (char * ) malloc ((n + 1) * sizeof(char));
    for (int i = 0; i <= n; ++i) BACK_TRACE[i] = 0;

    // We loop all the jobs (sorted by finishing time)
    for (int job_idx = 1; job_idx <= n; ++job_idx)
    {
        // We get the weight of this job
        int weight = jobs[job_idx].weight;

        // Get latest compatible job idx
        // NOTE: latest_compatible_idx = 0 if none
        int latest_compatible_idx = LATEST_COMPATIBLE[job_idx];

        // Get latest compatible weight
        int latest_compatible_weight = MAXIMUM_WEIGHT[latest_compatible_idx];

        // Calculate overall weight
        int overall_weight = weight + latest_compatible_weight;

        // Test if we've got a higher weight
        if (MAXIMUM_WEIGHT[job_idx - 1] < overall_weight)
        {
            // Update maximum weight
            MAXIMUM_WEIGHT[job_idx] = overall_weight;
            // Update back trace flag
            BACK_TRACE[job_idx] = 1; // set to true
        }
        else
        {
            // If including this job is not a good idea
            // we don't include it and use previous MAXIMUM_WEIGHT
            MAXIMUM_WEIGHT[job_idx] = MAXIMUM_WEIGHT[job_idx - 1];
            // NOTE: BACK_TRACE[job_idx] is preset to 0
        }
    }

    // Prepare the result
    wis_result result;
    result.max_weight = MAXIMUM_WEIGHT[n];
    // Allocate memory for jobs_included list
    result.jobs_included = (int *) malloc (n * sizeof(int));
    for (int i = 0; i < n; ++i) result.jobs_included[i] = 0;

    // Trace back to get the jobs included in the optimal solution
    int current_job_idx = n;
    int included_job_count = 0;
    while (current_job_idx > 0)
    {
        if (BACK_TRACE[current_job_idx] == 1)
        {
            // This job is included in the optimal solution
            result.jobs_included[included_job_count] = jobs[current_job_idx].job_id;
            ++included_job_count;
            // Move to the latest compatible job
            current_job_idx = LATEST_COMPATIBLE[current_job_idx];
        }
        else
        {
            // Move to the previous job
            --current_job_idx;
        }
    }

    // Free the allocated memory for jobs and LATEST_COMPATIBLE, MAXIMUM_WEIGHT, BACK_TRACE
    free(jobs);
    free(LATEST_COMPATIBLE);
    free(MAXIMUM_WEIGHT);
    free(BACK_TRACE);

    return result;
}

// Entry point
int alg_dp_weighted_interval_scheduling_main(int argc, char* argv[])
{
    // Initialize the jobs
    job jobs[] = {
        {1, 0, 6, 3},   // job 1: start_time = 0, finish_time = 6, weight = 3
        {2, 1, 4, 1},   // job 2: start_time = 1, finish_time = 4, weight = 1
        {3, 3, 5, 4},   // job 3: start_time = 3, finish_time = 5, weight = 4
        {4, 3, 8, 17},  // job 4: start_time = 3, finish_time = 8, weight = 17
        {5, 4, 7, 9},   // job 5: start_time = 4, finish_time = 7, weight = 9
        {6, 5, 9, 10},  // job 6: start_time = 5, finish_time = 9, weight = 10
        {7, 6, 10, 8},  // job 7: start_time = 6, finish_time = 10, weight = 8
        {8, 8, 11, 1}   // job 8: start_time = 8, finish_time = 11, weight = 1
    };
    int n = sizeof(jobs) / sizeof(jobs[0]);

    // Get the result
    wis_result result = weighted_interval_scheduling(jobs, n);

    // Print the result
    printf("\033[1;32m>>> Weighted Interval Scheduling Problem [RESULT] >>>\033[0m\n");
    printf("  ├── \033[1mJobs:\033[0m\n");
    for (int i = 0; i < n; ++i)    {
        printf("  │   ├── job(%d): start_time = %02d, finish_time = %02d, weight = %02d\n", i + 1, jobs[i].start_time, jobs[i].finish_time, jobs[i].weight);
    }
    printf("  ├── \033[1mMaximum total weight of non-overlapping jobs:\033[0m %d\n", result.max_weight);
    printf("  └── \033[1mJobs included in the optimal solution:\033[0m ");
    for (int i = 0; result.jobs_included[i] != 0; ++i) {
        int job_index = result.jobs_included[i];
        printf("job(%d) ", job_index);
    }
    printf("\n");

    // Free the allocated memory
    free(result.jobs_included);

    return 0;
}