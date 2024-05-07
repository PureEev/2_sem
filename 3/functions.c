#define _CRT_SECURE_NO_WARNINGS
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
bool alive;
bool gamestart;
Snake* previous = NULL;
Snake* Header = NULL;
int direction;
Frog frog;
Stone stone;
HWND hWnd;
int score = 0; // Scoring system

Snake* CreateBlockSnake(Snake* next, int i)
{
    Snake* previous = malloc(sizeof(Snake));
    previous->pos.x = (FirstPosX + (i * CellSize));
    previous->pos.y = FirstPosY;
    previous->SnakeLength = StartLength;
    previous->next = next;
    return previous;
}

void DrawButtons(HDC hdc, int Highlight) {
    RECT choose;
    RECT rectStart = { 200, 200, 320, 300 };
    HBRUSH brushStart = CreateSolidBrush(RGB(183, 255, 207));
    FillRect(hdc, &rectStart, brushStart);
    DrawText(hdc, L"Старт", -1, &rectStart, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    RECT rectExit = { 200, 320, 320, 420 };
    HBRUSH brushExit = CreateSolidBrush(RGB(183, 255, 207));
    FillRect(hdc, &rectExit, brushExit);
    DrawText(hdc, L"Выход", -1, &rectExit, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // Переменная для хранения текста кнопки
    LPCWSTR buttonText = L"";

    if (Highlight == 1) {
        HBRUSH brush1 = CreateSolidBrush(RGB(0, 255, 0));
        FillRect(hdc, &rectStart, brush1);
        DeleteObject(brush1);
        buttonText = L"Старт";
        choose = rectStart;
    }
    else if (Highlight == 2) {
        HBRUSH brush2 = CreateSolidBrush(RGB(0, 255, 0));
        FillRect(hdc, &rectExit, brush2);
        DeleteObject(brush2);
        buttonText = L"Выход";
        choose = rectExit;
    }
    // Восстановление текста кнопки
    DrawText(hdc, buttonText, -1, &choose, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void GenerateStone()
{
    int x, y;
    bool collision;
    do {
        x = (rand() % QuantityCells) * CellSize;
        y = (rand() % QuantityCells) * CellSize;
        collision = false;

        // Check collision with snake
        Snake* cur = previous;
        while (cur != NULL) {
            if (cur->pos.x == x && cur->pos.y == y) {
                collision = true;
                break;
            }
            cur = cur->next;
        }

        // Check collision with frog
        if (x == frog.pos.x && y == frog.pos.y) {
            collision = true;
        }
    } while (collision);

    stone.pos.x = x;
    stone.pos.y = y;
    stone.isActive = true;
    stone.duration = 10; // Set initial duration for stone appearance
}




bool CheckCollisionWithStone(HDC hdc, int Highlight)
{
    if (Header->pos.x == stone.pos.x && Header->pos.y == stone.pos.y) {
        // Snake collided with a stone, game over
        GameOver(hdc, Highlight);
        return true;
    }
    return false;
}

void GenerateFrog()
{
    int x, y;
    bool collision;
    do {
        x = (rand() % QuantityCells) * CellSize;
        y = (rand() % QuantityCells) * CellSize;
        collision = false;

        // Check collision with snake
        Snake* cur = previous;
        while (cur != NULL) {
            if (cur->pos.x == x && cur->pos.y == y) {
                collision = true;
                break;
            }
            cur = cur->next;
        }
    } while (collision);

    frog.pos.x = x;
    frog.pos.y = y;
    frog.duration = 30;
    frog.isActive = true;
}
void RestartGame()
{
    // Reset snake
    Snake* cur = previous;
    while (cur != NULL) {
        Snake* temp = cur;
        cur = cur->next;
        free(temp);
    }
    previous = NULL;
    Header = NULL;

    // Reset frog and stone
    frog.isActive = false;
    stone.isActive = false;

    // Reset score
    score = 0;

    // Reset direction
    direction = 0;

    // Restart the game
    SysteamInit(hWnd);
}

void GameOver(HDC hdc, int Highlight) {
    alive = false;
    static bool gameOver = false; // Флаг, указывающий на состояние игры (завершена или нет)
        if (gameOver) {
            return;  // Если игра уже завершена, выходим из функции
        }
    gameOver = true;  // Устанавливаем флаг, что игра завершена

    KillTimer(hWnd, 1);

    char scoreStr[20];
    sprintf(scoreStr, "Score: %d", score);
    wchar_t wideScoreStr[20];
    swprintf(wideScoreStr, sizeof(wideScoreStr) / sizeof(wideScoreStr[0]), L"%hs", scoreStr);

    MessageBox(hWnd, wideScoreStr, L"Game Over", MB_OK | MB_ICONINFORMATION);

    int result = MessageBox(hWnd, L"Do you want to play again?", L"Restart", MB_YESNO | MB_ICONQUESTION);

    if (result == IDYES) {
        alive = true;
        RestartGame();
        gameOver = false;  // Сбрасываем флаг, чтобы позволить вызов GameOver() в будущем
    }
    else if (result == IDNO) {
        gamestart = false;
        DrawButtons(hdc, Highlight);
    }
}
bool CheckCollisionWithFrog()
{
    if (Header->pos.x == frog.pos.x && Header->pos.y == frog.pos.y) {
        return true;
    }
    return false;
}



void DrawField(HDC hdc) {
    static HBRUSH hBrush1 = NULL;
    static HBRUSH hBrush2 = NULL;
    static HBRUSH hBrush3 = NULL;
    static HBRUSH hBrush4 = NULL;
    static HDC memDC = NULL;
    static HBITMAP hBitmap = NULL;
    static HBITMAP hOldBitmap = NULL;

    if (memDC == NULL) {
        // Create double buffer
        memDC = CreateCompatibleDC(hdc);
        hBitmap = CreateCompatibleBitmap(hdc, FieldSize, FieldSize);
        hOldBitmap = SelectObject(memDC, hBitmap);

        // Create brushes for colors
        hBrush1 = CreateSolidBrush(RGB(140, 207, 127)); // Color for even cells
        hBrush2 = CreateSolidBrush(RGB(58, 153, 68)); // Color for odd cells
        hBrush3 = CreateSolidBrush(RGB(0, 0, 0)); // Color for stones
        hBrush4 = CreateSolidBrush(RGB(125, 110, 215)); // Color for snake
    }

    // Draw the field in the double buffer
    for (int i = 0; i < QuantityCells; i++) {
        for (int j = 0; j < QuantityCells; j++) {
            HBRUSH hBrush = ((i + j) % 2 == 0) ? hBrush1 : hBrush2;
            SelectObject(memDC, hBrush);
            int x = i * CellSize;
            int y = j * CellSize;
            Rectangle(memDC, x, y, x + CellSize, y + CellSize);
        }
    }

    // Draw the snake in the double buffer
    Snake* cur = Header;
    SelectObject(memDC, hBrush4);
    while (cur != NULL) {
        Rectangle(memDC, cur->pos.x, cur->pos.y, cur->pos.x + CellSize, cur->pos.y + CellSize);
        cur = cur->next;
    }


    // Draw the frog in the double buffer
 if (frog.isActive) {
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); // Color for frog
    SelectObject(memDC, hBrush);
    Ellipse(memDC, frog.pos.x, frog.pos.y, frog.pos.x + CellSize, frog.pos.y + CellSize);
    DeleteObject(hBrush);
}

    // Draw the stone in the double buffer
    if (stone.isActive) {
        SelectObject(memDC, hBrush3);
        Rectangle(memDC, stone.pos.x, stone.pos.y, stone.pos.x + CellSize, stone.pos.y + CellSize);
    }

    // Copy the double buffer to the screen
    BitBlt(hdc, 0, 0, FieldSize, FieldSize, memDC, 0, 0, SRCCOPY);

}

void EatFrog()
{
    // Increase snake length
    Snake* newBlock = CreateBlockSnake(NULL, 0);
    Snake* cur = previous;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = newBlock;

    // Generate a new frog
    GenerateFrog();
    
    // Increase score
    score += 10;
}

void Move(HDC hdc, int Highlight)
{
    Snake* cur = Header;
    POINT previousPos = cur->pos; // Store the previous position of the snake's head

    // Update the position of the snake's head based on the direction
    int prevX = Header->pos.x;
    int prevY = Header->pos.y;
    int newPosX = Header->pos.x;
    int newPosY = Header->pos.y;


    if (direction == 0 && direction != 1 && Header->pos.y > 0) { // Up
        Header->pos.y -= CellSize;
        direction = 0; // Update the direction
    }
    else if (direction == 1 && direction != 0 && Header->pos.y < (FieldSize - CellSize)) { // Down
        Header->pos.y += CellSize;
        direction = 1; // Update the direction
    }
    else if (direction == 2 && direction != 3 && Header->pos.x > 0) { // Left
        Header->pos.x -= CellSize;
        direction = 2; // Update the direction
    }
    else if (direction == 3 && direction != 2 && Header->pos.x < (FieldSize - CellSize)) { // Right
        Header->pos.x += CellSize;
        direction = 3; // Update the direction
    }

    Snake* Cur = previous->next;
    while (Cur != NULL) {
        if (newPosX == Cur->pos.x && newPosY == Cur->pos.y) {
            // The snake collided with its own body, game over
            GameOver(hdc, Highlight);
            return;
        }
        Cur = Cur->next;
    }

    // Check collision with walls
    if (newPosX < 0 || newPosX >= FieldSize || newPosY < 0 || newPosY >= FieldSize) {
        // The snake went out of bounds, game over
        GameOver(hdc, Highlight);
        return;
    }

    // Update the positions of the rest of the snake's blocks
    while (cur->next != NULL)
    {
        cur = cur->next;
        POINT temp = cur->pos;
        cur->pos = previousPos;
        previousPos = temp;
    }

    // Check collision with frog
    if (CheckCollisionWithFrog()) {
        EatFrog();
        stone.duration = 15;
    }

    // Check collision with stone
    if (CheckCollisionWithStone(hdc, Highlight)) {
        // Handle collision with stone (e.g., game over)
        GameOver(hdc, Highlight);
        return;
    }

    // Update stone duration
    if (stone.isActive) {
        stone.duration--;
        if (stone.duration <= 0) {
            stone.isActive = false;
            GenerateStone();
        }
    }
}

void SysteamInit(HWND hWnd)
{
    SetTimer(hWnd, 1, 200, NULL);
    for (int i = StartLength; i >= 0; i--)
    {
        previous = CreateBlockSnake(previous, i);
    }
    Header = previous;
    Header->SnakeHead = true;
    GenerateFrog();
    GenerateStone(); // Generate the initial stone
}
 
void update(HDC hdc, bool gamestart, int Highlight)
{
    Move(alive, hdc, Highlight);
    DrawField(hdc);
}

