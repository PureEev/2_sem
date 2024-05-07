#include "asserts.h"
#define _CRT_SECURE_NO_WARNINGS

struct IgnoredAssert {
    int line;
    char* file;
    struct IgnoredAssert* prev;
    struct IgnoredAssert* next;
};

struct IgnoredAssert* AssertsHead = NULL;

void Assert(bool condition, const char* message, const char* file, int line, int MaxAsserts)
{
    static int count = 0;
    int numRetries = 0;
    bool retry = true;
    while (retry && (numRetries < MaxAsserts))
    {
        if (!condition)
        {
            char assertionMessage[512];
            snprintf(assertionMessage, sizeof(assertionMessage), "Assertion Failed!\n\nFile: %s\nLine: %d\n\n%s", file, line, message);
            // проверяем, не был ли ассерт уже проигнорирован
            bool alreadyIgnored = false;
            for (struct IgnoredAssert* prev = AssertsHead; prev != NULL; prev = prev->next)
            {
                if ((strcmp(prev->file, file) == 0)&&(prev->line == line))
                {
                    alreadyIgnored = true;
                    return 3;
                }
            }
            int choice = MessageBoxA(NULL, assertionMessage, "Assertion Failed!", MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_TOPMOST);
            if (choice == IDABORT)
            {
                ExitProcess(EXIT_FAILURE);
            }
            else if (choice == IDIGNORE)
            {
                retry = false;
            }
            else if (choice == IDRETRY)
            {
                // создаем новый элемент структуры IgnoredAssert и добавляем его в список игнорируемых ассертов
                struct IgnoredAssert* newAssert = malloc(sizeof(struct IgnoredAssert));
                newAssert->file = malloc(strlen(file) + 1 );
                strcpy_s(newAssert->file, strlen(file) + 1 , file);
                newAssert->line = line;
                newAssert->prev = NULL;
                newAssert->next = AssertsHead;
                if (AssertsHead != NULL)
                {
                    AssertsHead->prev = newAssert;
                }
                AssertsHead = newAssert;
            }
        }
        else
        {
            retry = false;
        }
        numRetries++;
    }
}
void FatalError(bool condition, const char* message, int returnValue)
{
    if (!condition)
    {
        MessageBoxA(NULL, message, "Fatal Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
        ExitProcess(returnValue);
    }
}

