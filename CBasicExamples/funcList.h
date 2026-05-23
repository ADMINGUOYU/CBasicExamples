#ifndef _FUNC_LIST_H_
#define _FUNC_LIST_H_

#include "helloWorld.h"
#include "recursion.h"
#include "memoryAlloc.h"
#include "bufferedInput.h"
#include "simpleCalculation.h"
#include "funcPointer.h"
#include "fileIO.h"
#include "forLoopC99.h"
#include "enumeration.h"
#include "large_uint_mul.h"
#include "array_demo.h"

// Algorithm
#include "Algorithm/Dynamic-Programming/rod_cutting.h"
#include "Algorithm/Dynamic-Programming/minimum_number_of_coins.h"
#include "Algorithm/Dynamic-Programming/weighted_interval_scheduling.h"
#include "Algorithm/Dynamic-Programming/highway_billboards.h"
#include "Algorithm/Dynamic-Programming/minimum_steps_to_one.h"
#include "Algorithm/Dynamic-Programming/longest_increasing_path.h"
#include "Algorithm/Dynamic-Programming/longest_increasing_subsequence.h"
#include "Algorithm/Dynamic-Programming/restricted_maxsum.h"
#include "Algorithm/Dynamic-Programming/2D_0-1_knapsack.h"
#include "Algorithm/Dynamic-Programming/2D_longest_common_sequence.h"
#include "Algorithm/Dynamic-Programming/2D_edit_distance.h"
#include "Algorithm/Dynamic-Programming/2D_egg_dropping.h"
#include "Algorithm/Dynamic-Programming/INT_longest_palindromic_substring.h"
#include "Algorithm/Greedy/huffman.h"
#include "Algorithm/Greedy/wifi_tower.h"
#include "Algorithm/Graph/graph_representation.h"
#include "Algorithm/Graph/graph_traversal.h"

char* programList[] = {
    "Hello World", 
    "recursion",
    "memory allocation",
    "buffered input",
    "simple calculator",
    "function pointer",
    "file I/O",
    "for loop scope C99 DEMO",
    "enumeration",
    "large number(uint) multiplication",
    "array demonstration",
    "Algorithm: Dynamic Programming - Rod Cutting Problem",
    "Algorithm: Dynamic Programming - Minimum Number of Coins",
    "Algorithm: Dynamic Programming - Weighted Interval Scheduling",
    "Algorithm: Dynamic Programming - Highway Billboards Problem",
    "Algorithm: Dynamic Programming - Minimum Steps To 1",
    "Algorithm: Dynamic Programming - Longest Increasing Path (TOP DOWN with memorization)",
    "Algorithm: Dynamic Programming - Longest Increasing Subsequence",
    "Algorithm: Dynamic Programming - Restricted Max-Sum Problem",
    "Algorithm: Dynamic Programming - [2D] 0/1 Knapsack Problem",
    "Algorithm: Dynamic Programming - [2D] Longest Common Subsequence",
    "Algorithm: Dynamic Programming - [2D] Edit Distance",
    "Algorithm: Dynamic Programming - [2D] Egg Dropping Problem",
    "Algorithm: Dynamic Programming - [INTERVAL] Longest Palindromic Substring",
    "Algorithm: Greedy - Huffman Coding",
    "Algorithm: Greedy - WiFi Tower Placement",
    "Algorithm: Graph - Adjacency Matrix/List Representation",
    "Algorithm: Graph - Traversal (BFS and DFS)"
};
int (*programFunc[])(int argc, char** argv) = {
    helloWorldMain, 
    recursionMain, 
    memAllocMain, 
    bufferedInputMain, 
    calculatorMain, 
    funcPointerMain, 
    fileIOMain, 
    forLoopC99Main, 
    enumerationMain,
    large_uint_mul_main,
    array_demo_main,
    alg_dp_rod_cutting_main,
    alg_dp_minimum_number_of_coins_main,
    alg_dp_weighted_interval_scheduling_main,
    alg_dp_highway_billboards_main,
    alg_dp_minimum_steps_to_one_main,
    alg_dp_longest_increasing_path_main,
    alg_dp_longest_increasing_subsequence_main,
    alg_dp_restricted_maxsum_main,
    alg_dp_2d_0_1_knapsack_main,
    alg_dp_2d_longest_common_sequence_main,
    alg_dp_2d_edit_distance_main,
    alg_dp_2d_egg_dropping_main,
    alg_dp_int_longest_palindromic_substring_main,
    alg_greedy_huffman_main,
    alg_greedy_wifi_tower_main,
    alg_graph_representation_main,
    alg_graph_traversal_main
};

// Define the program count
#define ProgramCount (sizeof(programList) / sizeof(programList[0]))

#endif