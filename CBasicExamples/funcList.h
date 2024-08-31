#pragma once
#include "helloWorld.h"
#include "recursion.h"
#include "memoryAlloc.h"
#include "bufferedInput.h"
#include "simpleCalculation.h"
#include "funcPointer.h"

#define ProgramCount 6
char* programList[] = {"Hello World", "recursion","memory allocation","buffered input","simple calculator","function pointer"};
void* programFunc[] = { helloWorldMain,recursionMain,memAllocMain,bufferedInputMain,calculatorMain,funcPointerMain};