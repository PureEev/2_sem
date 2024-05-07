#define _CRT_SECURE_NO_WARNINGS

#include "Functions.h"
#include <windows.h>
#include <stdio.h>


COLORREF color = 0x00000000;

void SaveScreenToBMP(const char* filename) {
    HDC hScreenDC = GetDC(NULL); // Получаем контекст устройства экрана

    // Устанавливаем размеры зоны захвата
    int captureWidth = 800;
    int captureHeight = 775;

    HDC hMemoryDC = CreateCompatibleDC(hScreenDC); // Создаем совместимый контекст устройства
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, captureWidth, captureHeight); // Создаем совместимый битмап

    // Выбираем битмап в контекст устройства
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);

    // Копируем изображение экрана в битмап с указанной зоной захвата
    BitBlt(hMemoryDC, 0, 0, captureWidth, captureHeight, hScreenDC, 0, 0, SRCCOPY);

    // Создаем структуру для сохранения изображения в формате BMP
    BITMAPINFOHEADER bmpInfoHeader;
    bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfoHeader.biWidth = captureWidth;
    bmpInfoHeader.biHeight = -captureHeight; // Отрицательная высота для сохранения изображения с верхнего левого угла
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitCount = 32; // 32-битный формат цвета
    bmpInfoHeader.biCompression = BI_RGB;
    bmpInfoHeader.biSizeImage = 0;
    bmpInfoHeader.biXPelsPerMeter = 0;
    bmpInfoHeader.biYPelsPerMeter = 0;
    bmpInfoHeader.biClrUsed = 0;
    bmpInfoHeader.biClrImportant = 0;

    // Создаем файл BMP и записываем в него заголовок и данные изображения
    FILE* file = fopen(filename, "wb");
    if (file != NULL) {
        BITMAPFILEHEADER bmpFileHeader;
        bmpFileHeader.bfType = 0x4D42; // Сигнатура "BM"
        bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + captureWidth * captureHeight * 4;
        bmpFileHeader.bfReserved1 = 0;
        bmpFileHeader.bfReserved2 = 0;
        bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

        fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, file);
        fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);

        // Получаем данные изображения из битмапа
        BITMAPINFO bmpInfo;
        bmpInfo.bmiHeader = bmpInfoHeader;
        BYTE* pPixels = NULL;
        HBITMAP hDIB = CreateDIBSection(hMemoryDC, &bmpInfo, DIB_RGB_COLORS, (void**)&pPixels, NULL, 0);
        if (hDIB != NULL) {
            GetDIBits(hMemoryDC, hBitmap, 0, captureHeight, pPixels, &bmpInfo, DIB_RGB_COLORS);
            fwrite(pPixels, sizeof(BYTE), captureWidth * captureHeight * 4, file);
            DeleteObject(hDIB);
        }

        fclose(file);
    }

    SelectObject(hMemoryDC, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);
}
void New(HDC hdc)
{
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    Rectangle(hdc, 0, 0, 800, 675);
}


COLORREF ChoseColor(HWND hWnd)
{
    CHOOSECOLOR cc;
    static COLORREF acrCustClr[16];
    ZeroMemory(&cc, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hWnd;
    cc.lpCustColors = (LPDWORD)acrCustClr;
    cc.rgbResult = color;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;
    ChooseColor(&cc);
    color = cc.rgbResult;
    return color;
}

void DrawLine(HDC hdc, int x1, int y1, int x2, int y2, int PenSize)
{
    HPEN hPen = CreatePen(PS_SOLID, PenSize, color);
    SelectObject(hdc, hPen);
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);
    DeleteObject(hPen);
}

void OpenBMPFile(HDC hdc, HWND hwnd)
{
    HBITMAP hBitmap1 = LoadImage(NULL, L"C:\\Users\\burla\\source\\repos\\2_6\\save.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!hBitmap1) {
        MessageBox(NULL, L"Failed to load image", L"Error", MB_OK | MB_ICONERROR);
        return 0;
    }

    // Получение контекста устройства окна
    hdc = GetDC(hwnd);

    // Создание совместимой битовой карты
    HDC hMemDC1 = CreateCompatibleDC(hdc);
    HBITMAP hOldBitmap1 = (HBITMAP)SelectObject(hMemDC1, hBitmap1);

    // Определение размеров изображения
    BITMAP bitmap1;
    GetObject(hBitmap1, sizeof(BITMAP), &bitmap1);
    int imageWidth = bitmap1.bmWidth;
    int imageHeight = bitmap1.bmHeight;

    // Копирование изображения на контекст устройства окна
    BitBlt(hdc, 0, 0, imageWidth, imageHeight, hMemDC1, 0, 0, SRCCOPY);

    // Освобождение ресурсов
    SelectObject(hMemDC1, hOldBitmap1);
    DeleteDC(hMemDC1);
    ReleaseDC(hwnd, hdc);
    DeleteObject(hBitmap1);
}