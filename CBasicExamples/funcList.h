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

#define ProgramCount 9
char* programList[] = {
    "Hello World", 
    "recursion",
    "memory allocation",
    "buffered input",
    "simple calculator",
    "function pointer",
    "file I/O",
    "for loop scope C99 DEMO",
    "enumeration"};
void* programFunc[] = {
    (void*)helloWorldMain, 
    (void*)recursionMain, 
    (void*)memAllocMain, 
    (void*)bufferedInputMain, 
    (void*)calculatorMain, 
    (void*)funcPointerMain, 
    (void*)fileIOMain, 
    (void*)forLoopC99Main, 
    (void*)enumerationMain};

#endif