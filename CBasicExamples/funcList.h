#pragma once
#include "helloWorld.h"
#include "recursion.h"
#include "memoryAlloc.h"
#include "bufferedInput.h"
#include "simpleCalculation.h"

#define ProgramCount 5
char* programList[] = {"Hello World", "recursion","memory allocation","buffered input","simple calculator"};
void* programFunc[] = { helloWorldMain,recursionMain,memAllocMain,bufferedInputMain,calculatorMain};