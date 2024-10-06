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

void AutoKeyPresser::ZlapIdOkna(HWND &handle, HWND &parentHandle)
{
    qDebug() << "AutoKeyPresser::ZlapIdOkna() został wywołany.";
    POINT P;
    GetCursorPos(&P);
    handle = WindowFromPoint(P);
    parentHandle = GetAncestor(handle, GA_ROOT);
    qDebug() << "handle to " << handle <<
                "- Tekst" << GetWindowTextFromHandle(handle);
    qDebug() << "parentHandle" << parentHandle <<
                "- Tekst" << GetWindowTextFromHandle(parentHandle) << "\n";
}
