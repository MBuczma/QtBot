/* AutoKeyPresser.cpp */
#include "AutoKeyPresser.h"

#include <QDebug>
#include <unordered_map>
#include <windows.h>

AutoKeyPresser::AutoKeyPresser() {}
AutoKeyPresser::~AutoKeyPresser() {}

void AutoKeyPresser::WindowHandleFromPoint(HWND &handle, HWND &parentHandle)
{
    qDebug() << "AutoKeyPresser::WindowHandleFromPoint() został wywołany.";
    POINT point;
    GetCursorPos(&point); // Pobranie współrzędnych kursora w odniesieniu do ekranu
    handle = WindowFromPoint(point);
    parentHandle = GetAncestor(handle, GA_ROOT);

    if (handle != nullptr) {
        qDebug() << "X:" << point.x << "Y:" << point.y;
        qDebug() << "Handle:" << handle << "- Tekst:" << GetWindowTextFromHandle(handle);
        qDebug() << "ParentHandle:" << parentHandle
                 << "- Tekst:" << GetWindowTextFromHandle(parentHandle) << "\n";
    } else {
        qDebug() << "Nie udało się pobrać uchwytu okna.";
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
        qDebug() << "Nie udało się pobrać tekstu dla uchwytu:" << hwnd;
        return QString(); // Zwraca pusty QString
    }
}


void AutoKeyPresser::SentKey(const HWND handle, const QString &key)
{
    // Mapa klawiszy jest statyczna, aby była tworzona tylko raz
    static const std::unordered_map<QString, WPARAM> keyMap
        = {{"Left", VK_LEFT},   {"Up", VK_UP},        {"Right", VK_RIGHT}, {"Down", VK_DOWN},
           {"Space", VK_SPACE}, {"Enter", VK_RETURN}, {"Esc", VK_ESCAPE},  {"F1", VK_F1},
           {"F2", VK_F2},       {"F3", VK_F3},        {"F4", VK_F4},       {"F5", VK_F5},
           {"F6", VK_F6},       {"F7", VK_F7},        {"F8", VK_F8},       {"F9", VK_F9},
           {"F10", VK_F10},     {"F11", VK_F11},      {"F12", VK_F12},     {"1", '1'},
           {"2", '2'},          {"3", '3'},           {"4", '4'},          {"5", '5'},
           {"6", '6'},          {"7", '7'},           {"8", '8'},          {"9", '9'},
           {"0", '0'}};

    auto it = keyMap.find(key);
    if (it != keyMap.end()) {
        WPARAM keyCode = it->second;
        PostMessage(handle, WM_KEYDOWN, keyCode, 0);
        PostMessage(handle, WM_KEYUP, keyCode, 0);
        qDebug() << "Wysyłam klawisz:" << key << "do uchwytu:" << handle;
    } else {
        qDebug() << "Nieznany klawisz:" << key;
    }
}
