/* AutoKeyPresser.cpp */
#include "AutoKeyPresser.h"
#include <windows.h>

AutoKeyPresser::AutoKeyPresser(QWidget *parent)
    : QWidget{parent}
{

}

AutoKeyPresser::~AutoKeyPresser()
{

}

QString AutoKeyPresser::GetWindowTextFromHandle(HWND hwnd)
{
    // Bufor na przechowanie tekstu okna
    wchar_t windowText[256];  // Maksymalna długość tekstu to 256 znaków
    int length = GetWindowTextW(hwnd, windowText, sizeof(windowText) / sizeof(wchar_t));

    if (length > 0){
        // Zamieniamy tekst z wide-char (wchar_t) na QString
        return QString::fromWCharArray(windowText, length);
    } else {
        return QString("Nie udało się pobrać tekstu.");
    }
}

void AutoKeyPresser::WindowHandleFromPoint(HWND &handle, HWND &parentHandle)
{
    qDebug() << "AutoKeyPresser::WindowHandleFromPoint() został wywołany.";
    POINT P;
    GetCursorPos(&P);
    ScreenToClient(handle, &P);  // Przelicz na współrzędne okna
    handle = WindowFromPoint(P);
    parentHandle = GetAncestor(handle, GA_ROOT);
    qDebug() << "X:" << P.x << "Y:" << P.y;
    qDebug() << "handle to " << handle <<
                "- Tekst" << GetWindowTextFromHandle(handle);
    qDebug() << "parentHandle" << parentHandle <<
                "- Tekst" << GetWindowTextFromHandle(parentHandle) << "\n";
}

void AutoKeyPresser::SentKey(HWND &handle, QString key)
{
    std::map<QString, WPARAM> keyMap = {
        {"1", '1'},
        {"2", '2'},
        {"A", 'A'},
        {"B", 'B'},
        {"SPACE", VK_SPACE},
        {"ENTER", VK_RETURN},
        // Dodaj inne klawisze, które chcesz obsłużyć
    };

    PostMessage(handle, WM_KEYDOWN, keyMap[key], 0);
    PostMessage(handle, WM_KEYUP, keyMap[key], 0);
}
