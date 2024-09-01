#pragma once
#include "helloWorld.h"
#include "recursion.h"
#include "memoryAlloc.h"
#include "bufferedInput.h"
#include "simpleCalculation.h"
#include "funcPointer.h"
#include "fileIO.h"

#define ProgramCount 7
char* programList[] = {"Hello World", "recursion","memory allocation","buffered input","simple calculator","function pointer","file I/O"};
void* programFunc[] = {helloWorldMain, recursionMain, memAllocMain, bufferedInputMain, calculatorMain, funcPointerMain, fileIOMain};