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
    "Algorithm: Dynamic Programming - Weighted Interval Scheduling"
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
    alg_dp_weighted_interval_scheduling_main
};

// Define the program count
#define ProgramCount (sizeof(programList) / sizeof(programList[0]))

#endif