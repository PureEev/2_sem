#pragma once
#include "framework.h"
#include "stdbool.h"
#define FieldSize 680
#define CellSize 40
#define QuantityCells 16
#define FirstPosX 360
#define FirstPosY 320
#define StartLength 0
#define BUTTON_START_ID 1
#define BUTTON_EXIT_ID 2



typedef struct {
    int SnakeLength;
    bool SnakeHead;
    POINT pos;
    struct Snake* next;
} Snake;


typedef struct {
    POINT pos;
    bool isActive;
    int duration;
} Stone;

typedef struct {
    POINT pos;
    bool isActive;
    int duration;
} Frog;

void SysteamInit(HWND hWnd);
void update(HDC hdc, int Highlight);
void DrawButtons(HDC hdc, int Highlight);
void GameOver(HDC hdc, int Highlight);