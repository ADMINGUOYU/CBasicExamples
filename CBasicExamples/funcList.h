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

#define ProgramCount 11
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
    "array demonstration"
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
    array_demo_main
};

#endif