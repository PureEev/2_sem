#pragma once
#include <Windows.h>
#include <stdio.h>
#include <stdbool.h>
#define Max 1024
#ifdef NDEBUG
#define assert(expression) ((void)0)
#else
#define assert(expression) Assert((expression), #expression, __FILE__, __LINE__, Max)
#endif
#define fatal_error(expression, returnValue) FatalError((expression), #expression, returnValue)

void Assert(bool condition, const char* message, const char* file, int line, int MaxAsserts);
void FatalError(bool condition, const char* message, int returnValue);