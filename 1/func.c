#include "func.h"
#include <windows.h>
#include "Windowsx.h"
#include "math.h"

void DrawTriangle (HDC hdc, Triangle triangle) 
{
	MoveToEx(hdc, triangle.side1.x, triangle.side1.y, NULL);   // Переместить перо в начало первой линии
	LineTo(hdc, triangle.side2.x, triangle.side2.y);            // Нарисовать первую линию
	LineTo(hdc, triangle.side3.x, triangle.side3.y);            // Нарисовать вторую линию
	LineTo(hdc, triangle.side1.x, triangle.side1.y);            // Закончить третью линию в начале первой
}

void drawSierpinskiTriangle(HDC hdc, Triangle triangle, int depth)
{
	if (depth == 0)
	{
		DrawTriangle(hdc, triangle);
	}
	else
	{
		POINT new12 = { (triangle.side1.x + triangle.side2.x) / 2,
			(triangle.side1.y + triangle.side2.y) / 2};
		POINT new23 = { (triangle.side3.x + triangle.side2.x) / 2,
			(triangle.side3.y + triangle.side2.y) / 2 };
		POINT new31 = { (triangle.side1.x + triangle.side3.x) / 2,
			(triangle.side1.y + triangle.side3.y) / 2 };
		Triangle Left = 
		{
			triangle.side1, new12, new31
		};
		Triangle Right = 
		{
			new12, triangle.side2, new23
		};
		Triangle Up = 
		{
			new31, new23, triangle.side3
		};
		drawSierpinskiTriangle( hdc, Left, depth - 1);
		drawSierpinskiTriangle( hdc, Right, depth - 1);
		drawSierpinskiTriangle( hdc, Up, depth - 1);
	}
}