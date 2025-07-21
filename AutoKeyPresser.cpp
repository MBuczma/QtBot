/* AutoKeyPresser.cpp */
#include "AutoKeyPresser.h"

#include <QDebug>
#include <KeyMap.h>
#include <windows.h>

AutoKeyPresser::AutoKeyPresser() {}
AutoKeyPresser::~AutoKeyPresser() {}

void AutoKeyPresser::WindowHandleFromPoint(HWND &handle, HWND &parentHandle)
{
    qDebug() << "[AutoKeyPresser] WindowHandleFromPoint() został wywołany.";
    POINT point;
    GetCursorPos(&point); // Pobranie współrzędnych kursora w odniesieniu do ekranu
    handle = WindowFromPoint(point);
    parentHandle = GetAncestor(handle, GA_ROOT);

    if (handle != nullptr) {
        //qDebug() << "X:" << point.x << "Y:" << point.y;
        qDebug() << "[AutoKeyPresser] Handle:" << handle
                 << "- Tekst:" << GetWindowTextFromHandle(handle);
        qDebug() << "[AutoKeyPresser] ParentHandle:" << parentHandle
                 << "- Tekst:" << GetWindowTextFromHandle(parentHandle) << "\n";
    } else {
        qDebug() << "[AutoKeyPresser] Nie udało się pobrać uchwytu okna.";
    }
}

QString AutoKeyPresser::GetWindowTextFromHandle(const HWND hwnd) const
{
    // Bufor na przechowanie tekstu okna
    wchar_t windowText[256]; // Maksymalna długość tekstu to 256 znaków
    int length = GetWindowTextW(hwnd, windowText, sizeof(windowText) / sizeof(wchar_t));

    if (length > 0) {
        // Zamieniamy tekst z wide-char (wchar_t) na QString
        return QString::fromWCharArray(windowText, length);
    } else {
        qDebug() << "[AutoKeyPresser] Nie udało się pobrać tekstu dla uchwytu:" << hwnd;
        return QString(); // Zwraca pusty QString
    }
}

void AutoKeyPresser::SentKey(const HWND handle, const QString &key)
{
    const auto &keyMap = KeyMap::getMap();
    QString keyUpper = key.trimmed().toUpper();

    auto it = keyMap.find(keyUpper);
    if (it != keyMap.end()) {
        WPARAM keyCode = it.value();
        PostMessage(handle, WM_KEYDOWN, keyCode, 0);
        PostMessage(handle, WM_KEYUP, keyCode, 0);
        qDebug() << "[AutoKeyPresser] Wysyłam klawisz:" << key << "do uchwytu:" << handle;
    } else {
        qDebug() << "[AutoKeyPresser] Nieznany klawisz:" << key
                 << "(po przekształceniu:" << keyUpper << ")";
    }
}
