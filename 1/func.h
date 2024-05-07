#pragma once
#include "framework.h"
typedef struct
{
	POINT side1;
	POINT side2;
	POINT side3;

}Triangle;
void DrawTriangle(hdc, triangle);
void drawSierpinskiTriangle(hdc, triangle, depth);