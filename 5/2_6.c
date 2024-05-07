#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "2_6.h"
#include "Functions.h"
#include "Windows.h"
#include <stdio.h>
#include <stdbool.h>
int PenSize = 3;
char* file;
bool push;
#define MAX_LOADSTRING 100
COLORREF color;
POINT g_startPt; // Начальная точка для рисования
BOOL g_isDrawing = FALSE;
HWND  inputField;
int g_startX, g_startY, endX, endY;
char buffer[256];
int result, number;
HWND hEdit;
char input[256];
// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

HWND hWndButtonSave;                            // Кнопка "Сохранить"
HWND hWndButtonOpen;                            // Кнопка "Открыть"
HWND hWndButtonColor;                           // Кнопка "Выбрать цвет"
HWND hWndButtonNew;
HWND hWndButtonEraser;
HWND hWndButtonChoose;
HWND hWndButtonPlus;
HWND hWndButtonMinus;
HWND hWndButtonCurent;

// Отрисовка кнопки

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY26, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY26));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY26));
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY26);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        0, 0, 850, 800, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    RECT rectClient;
    GetClientRect(hWnd, &rectClient);

    // Вычисление размеров кнопок
    int buttonWidth = rectClient.right / 9;
    int buttonHeight = 50;

    // Расположение кнопок
    int x = 0;
    int y = rectClient.bottom - buttonHeight;

    // Создание кнопки "Сохранить"
    hWndButtonSave = CreateWindow(L"BUTTON", L"Сохранить", WS_VISIBLE | WS_CHILD,
        x, y, buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_SAVE_ID, hInst, NULL);

    // Создание кнопки "Открыть"
    hWndButtonOpen = CreateWindow(L"BUTTON", L"Открыть", WS_VISIBLE | WS_CHILD,
        x + buttonWidth, y, buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_OPEN_ID, hInst, NULL);

    // Создание кнопки "Выбрать цвет
    hWndButtonColor = CreateWindow(L"BUTTON", L"Выбрать цвет", WS_VISIBLE | WS_CHILD,
        x + buttonWidth * 2, y, buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_COLOR_ID, hInst, NULL);// Создание кнопок
    hWndButtonNew = CreateWindow(L"BUTTON", L"Новый", WS_VISIBLE | WS_CHILD,
        x + buttonWidth * 3, y, buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_NEW_ID, hInst, NULL);
    hWndButtonEraser = CreateWindow(L"BUTTON", L"Ластик", WS_VISIBLE | WS_CHILD,
        x + buttonWidth * 4, y, buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_ERASER_ID, hInst, NULL);
    hWndButtonChoose = CreateWindow(L"BUTTON", L"Ручка", WS_VISIBLE | WS_CHILD,
        x + buttonWidth * 5, y, buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_CHOOSE_ID, hInst, NULL);

        hWndButtonPlus = CreateWindow(L"BUTTON", L"+", WS_VISIBLE | WS_CHILD,
            x + buttonWidth * 6, y, buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_PLUS_ID, hInst, NULL);

        // Создание кнопки "-"
        hWndButtonMinus = CreateWindow(L"BUTTON", L"-", WS_VISIBLE | WS_CHILD,
            x + buttonWidth * 7, y, buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_MINUS_ID, hInst, NULL);

        // Создание кнопки "Текущее"
        hWndButtonCurent = CreateWindow(L"BUTTON", L"Стандарт", WS_VISIBLE | WS_CHILD,
            x + buttonWidth * 8, y, buttonWidth, buttonHeight, hWnd, (HMENU)BUTTON_CURENT_ID, hInst, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}
HDC hdc;
//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
    {
        g_isDrawing = TRUE; // Устанавливаем флаг, что начато рисование
        g_startPt.x = LOWORD(lParam); // Сохраняем начальную координату X
        g_startPt.y = HIWORD(lParam); // Сохраняем начальную координату Y
        SetCapture(hWnd); // Захватываем мышь
        break;
    }
    case WM_LBUTTONUP:
    {
        if (LOWORD(lParam) > DRAWING_AREA_WIDTH || HIWORD(lParam) > DRAWING_AREA_HEIGHT)
            break;
        if (g_isDrawing)
        {
            g_isDrawing = FALSE; // Сбрасываем флаг, что рисование завершено
            ReleaseCapture(); // Освобождаем захват мыши

            HDC hdc = GetDC(hWnd);
            int endX = LOWORD(lParam); // Получаем координату X из lParam
            int endY = HIWORD(lParam); // Получаем координату Y из lParam
            DrawLine(hdc, g_startPt.x, g_startPt.y, endX, endY, PenSize); // Вызываем функцию рисования линии
            ReleaseDC(hWnd, hdc);
        }
        break;
    }
    case WM_MOUSEMOVE:
    {
        if (g_isDrawing)
        {
            HDC hdc = GetDC(hWnd);
            int endX = LOWORD(lParam); // Получаем координату X из lParam
            int endY = HIWORD(lParam); // Получаем координату Y из lParam
            if (endX > DRAWING_AREA_WIDTH || endY > DRAWING_AREA_HEIGHT)
                break;
            DrawLine(hdc, g_startPt.x, g_startPt.y, endX, endY, PenSize); // Вызываем функцию рисования линии
            ReleaseDC(hWnd, hdc);

            // Обновляем начальную точку для следующей линии
            g_startPt.x = endX;
            g_startPt.y = endY;
        }
        break;
    }
    case WM_COMMAND:
        
    {
        int wmId = LOWORD(wParam);
        // Разобрать выбор в меню:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case BUTTON_SAVE_ID:
            //MessageBox(hWnd, L"Кнопка 'Сохранить' нажата", L"Сохранить", MB_OK);
            SaveScreenToBMP("save.bmp");
            break;
        case BUTTON_OPEN_ID:
            OpenBMPFile(hdc, hWnd);
            //MessageBox(hWnd, L"Кнопка 'Открыть' нажата", L"Открыть", MB_OK);
            break;
        case BUTTON_COLOR_ID:
            color = ChoseColor(hWnd);
            break;
        case BUTTON_NEW_ID:
            hdc = GetDC(hWnd);
            New(hdc);
            ReleaseDC(hWnd, hdc);
            break;
        case BUTTON_CHOOSE_ID:
            color = 0x00000000;
            break;
        case BUTTON_ERASER_ID:
            color = 0x00FFFFFF;
            break;
        case BUTTON_MINUS_ID:
            PenSize -= 1;
            break;
        case BUTTON_PLUS_ID:
            PenSize += 1;
            break;
        case BUTTON_CURENT_ID:
            PenSize = 3;
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
        Rectangle(hdc, 0, 0, 800, 675);
        RECT rectSaveButton, rectOpenButton, rectColorButton, rectNewButton, rectEraserButton, rectPenButton, rectPlusButton, rectCurentButton, rectMinusButton;
        GetClientRect(hWnd, &rectSaveButton);
        rectSaveButton.right /= 9;
        GetClientRect(hWnd, &rectOpenButton);
        rectOpenButton.left = rectSaveButton.right;
        rectOpenButton.right = rectSaveButton.right * 2;
        GetClientRect(hWnd, &rectColorButton);
        rectColorButton.left = rectOpenButton.right;
        rectColorButton.right = rectOpenButton.right * 3;
        GetClientRect(hWnd, &rectNewButton);
        rectNewButton.left = rectColorButton.right;
        rectNewButton.right = rectColorButton.right * 5;
        GetClientRect(hWnd, &rectEraserButton);
        rectEraserButton.left = rectNewButton.right;
        rectEraserButton.right = rectNewButton.right * 6;
        GetClientRect(hWnd, &rectPenButton);
        rectPenButton.left = rectEraserButton.right;
        rectPenButton.right = rectEraserButton.right* 7;
        GetClientRect(hWnd, &rectPlusButton);
        rectPlusButton.left = rectPenButton.right;
        rectPlusButton.right = rectPenButton.right * 8;
        GetClientRect(hWnd, &rectCurentButton);
        rectCurentButton.left = rectPlusButton.right;
        rectCurentButton.right = rectPlusButton.right * 9;
        GetClientRect(hWnd, &rectMinusButton);
        rectMinusButton.left = rectCurentButton.right;
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
