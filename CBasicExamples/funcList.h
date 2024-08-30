#pragma once
#include "helloWorld.h"
#include "recursion.h"
#include "memoryAlloc.h"

#define ProgramCount 3
char* programList[] = {"Hello World", "recursion","memory allocation" };
void* programFunc[] = { helloWorldMain,recursionMain,memAllocMain };