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
        {"Left", VK_LEFT}, {"Up", VK_UP},
        {"Right", VK_RIGHT}, {"Down", VK_DOWN},
        {"Space", VK_SPACE}, {"Enter", VK_RETURN},
        {"Esc", VK_ESCAPE},
        {"F1", VK_F1}, {"F2", VK_F2},
        {"F3", VK_F3}, {"F4", VK_F4},
        {"F5", VK_F5}, {"F6", VK_F6},
        {"F7", VK_F7}, {"F8", VK_F8},
        {"F9", VK_F9}, {"F10", VK_F10},
        {"F11", VK_F11}, {"F12", VK_F12},
        {"1", '1'}, {"2", '2'},
        {"3", '3'}, {"4", '4'},
        {"5", '5'}, {"6", '6'},
        {"7", '7'}, {"8", '8'},
        {"9", '9'}, {"0", '0'}
    };

    PostMessage(handle, WM_KEYDOWN, keyMap[key], 0);
    PostMessage(handle, WM_KEYUP, keyMap[key], 0);
}
