#include "GlobalKeyListener.h"
#include <QDebug>
#include <QKeySequence>
#include <QMetaObject>

HHOOK GlobalKeyListener::hook = nullptr;
GlobalKeyListener *GlobalKeyListener::instance = nullptr;

GlobalKeyListener::GlobalKeyListener(QObject *parent)
    : QObject(parent)
{}

GlobalKeyListener::~GlobalKeyListener()
{
    stop();
}

void GlobalKeyListener::start()
{
    if (!hook) {
        instance = this;
        hook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardProc, nullptr, 0);
        if (!hook) {
            qWarning() << "[GlobalKeyListener] Nie udało się ustawić hooka!";
        } else {
            qDebug() << "[GlobalKeyListener] Hook aktywowany.";
        }
    }
}

void GlobalKeyListener::stop()
{
    if (hook) {
        UnhookWindowsHookEx(hook);
        hook = nullptr;
        instance = nullptr;
        qDebug() << "[GlobalKeyListener] Hook zatrzymany.";
    }
}

LRESULT CALLBACK GlobalKeyListener::keyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && wParam == WM_KEYDOWN && instance) {
        KBDLLHOOKSTRUCT *p = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);
        DWORD vkCode = p->vkCode;

        QMetaObject::invokeMethod(
            instance,
            [vkCode]() {
                QString keyText = opisVK(vkCode);
                qDebug() << "[GlobalKeyListener] Wciśnięto VK:" << vkCode
                         << (keyText.isEmpty() ? "" : QString("(%1)").arg(keyText));

                emit instance->keyPressed(vkCode);
            },
            Qt::QueuedConnection);
    }

    return CallNextHookEx(hook, nCode, wParam, lParam);
}

QString GlobalKeyListener::opisVK(int vk)
{
    static const QMap<int, QString> mapa = {// Litery A-Z
                                            {'A', "A"},
                                            {'B', "B"},
                                            {'C', "C"},
                                            {'D', "D"},
                                            {'E', "E"},
                                            {'F', "F"},
                                            {'G', "G"},
                                            {'H', "H"},
                                            {'I', "I"},
                                            {'J', "J"},
                                            {'K', "K"},
                                            {'L', "L"},
                                            {'M', "M"},
                                            {'N', "N"},
                                            {'O', "O"},
                                            {'P', "P"},
                                            {'Q', "Q"},
                                            {'R', "R"},
                                            {'S', "S"},
                                            {'T', "T"},
                                            {'U', "U"},
                                            {'V', "V"},
                                            {'W', "W"},
                                            {'X', "X"},
                                            {'Y', "Y"},
                                            {'Z', "Z"},

                                            // Cyfry główne
                                            {'0', "0"},
                                            {'1', "1"},
                                            {'2', "2"},
                                            {'3', "3"},
                                            {'4', "4"},
                                            {'5', "5"},
                                            {'6', "6"},
                                            {'7', "7"},
                                            {'8', "8"},
                                            {'9', "9"},

                                            // Klawisze numpada
                                            {VK_NUMPAD0, "Numpad 0"},
                                            {VK_NUMPAD1, "Numpad 1"},
                                            {VK_NUMPAD2, "Numpad 2"},
                                            {VK_NUMPAD3, "Numpad 3"},
                                            {VK_NUMPAD4, "Numpad 4"},
                                            {VK_NUMPAD5, "Numpad 5"},
                                            {VK_NUMPAD6, "Numpad 6"},
                                            {VK_NUMPAD7, "Numpad 7"},
                                            {VK_NUMPAD8, "Numpad 8"},
                                            {VK_NUMPAD9, "Numpad 9"},
                                            {VK_DECIMAL, "Numpad ."},
                                            {VK_ADD, "Numpad +"},
                                            {VK_SUBTRACT, "Numpad -"},
                                            {VK_MULTIPLY, "Numpad *"},
                                            {VK_DIVIDE, "Numpad /"},

                                            // Klawisze funkcyjne

                                            {VK_F1, "F1"},
                                            {VK_F2, "F2"},
                                            {VK_F3, "F3"},
                                            {VK_F4, "F4"},
                                            {VK_F5, "F5"},
                                            {VK_F6, "F6"},
                                            {VK_F7, "F7"},
                                            {VK_F8, "F8"},
                                            {VK_F9, "F9"},
                                            {VK_F10, "F10"},
                                            {VK_F11, "F11"},
                                            {VK_F12, "F12"},
                                            {VK_F13, "F13"},
                                            {VK_F14, "F14"},
                                            {VK_F15, "F15"},
                                            {VK_F16, "F16"},
                                            {VK_F17, "F17"},
                                            {VK_F18, "F18"},
                                            {VK_F19, "F19"},
                                            {VK_F20, "F20"},
                                            {VK_F21, "F21"},
                                            {VK_F22, "F22"},
                                            {VK_F23, "F23"},
                                            {VK_F24, "F24"},

                                            // Klawisze specjalne i międzynarodowe
                                            {VK_ESCAPE, "Esc"},
                                            {VK_TAB, "Tab"},
                                            {VK_BACK, "Backspace"},
                                            {VK_RETURN, "Enter"},
                                            {VK_SPACE, "Space"},
                                            {VK_CAPITAL, "Caps Lock"},
                                            {VK_LSHIFT, "Left Shift"},
                                            {VK_RSHIFT, "Right Shift"},
                                            {VK_LCONTROL, "Left Ctrl"},
                                            {VK_RCONTROL, "Right Ctrl"},
                                            {VK_LMENU, "Left Alt"},
                                            {VK_RMENU, "Right Alt"},
                                            {VK_INSERT, "Insert"},
                                            {VK_DELETE, "Delete"},
                                            {VK_HOME, "Home"},
                                            {VK_END, "End"},
                                            {VK_PRIOR, "Page Up"},
                                            {VK_NEXT, "Page Down"},
                                            {VK_LEFT, "Left Arrow"},
                                            {VK_RIGHT, "Right Arrow"},
                                            {VK_UP, "Up Arrow"},
                                            {VK_DOWN, "Down Arrow"},
                                            {VK_SNAPSHOT, "Print Screen"},
                                            {VK_SCROLL, "Scroll Lock"},
                                            {VK_PAUSE, "Pause"},

                                            // Znaki specjalne (OEM)
                                            {VK_OEM_1, "; :"},
                                            {VK_OEM_2, "/ ?"},
                                            {VK_OEM_3, "~ `"},
                                            {VK_OEM_4, "[ {"},
                                            {VK_OEM_5, "\\ |"},
                                            {VK_OEM_6, "] }"},
                                            {VK_OEM_7, "' \""},
                                            {VK_OEM_PLUS, "+ ="},
                                            {VK_OEM_COMMA, ", <"},
                                            {VK_OEM_MINUS, "- _"},
                                            {VK_OEM_PERIOD, ". >"}};

    return mapa.value(vk, QString("VK_%1").arg(vk));
}
