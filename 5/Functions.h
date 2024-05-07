#pragma once

#define BUTTON_SAVE_ID                  1 // Идентификатор кнопки "Сохранить"
#define BUTTON_OPEN_ID                  2 // Идентификатор кнопки "Открыть"
#define BUTTON_COLOR_ID                 3 // Идентификатор кнопки "Выбрать цвет"
#define BUTTON_NEW_ID                   4
#define BUTTON_ERASER_ID                5
#define BUTTON_CHOOSE_ID				6
#define BUTTON_PLUS_ID					7
#define BUTTON_MINUS_ID					8
#define BUTTON_CURENT_ID				9
#define DRAWING_AREA_WIDTH 800
#define DRAWING_AREA_HEIGHT 675

void Draw(hWnd, hdc, x, y, endX, endY);
void New(hdc);
void OpenBMPFile(hdc, hwnd);
void SaveScreenToBMP(filename);