#include "GlobalKeyListener.h"
#include <QDebug>
#include <QKeySequence>
#include <QMetaObject>
#include "KeyMap.h"

// Uchwyt do hooka systemowego (Windows Hook)
HHOOK GlobalKeyListener::hook = nullptr;

// Wskaźnik na aktualną instancję GlobalKeyListener (potrzebny dla callbacka)
GlobalKeyListener *GlobalKeyListener::instance = nullptr;

GlobalKeyListener::GlobalKeyListener(QObject *parent)
    : QObject(parent)
{}

GlobalKeyListener::~GlobalKeyListener()
{
    if (hook) {
        UnhookWindowsHookEx(hook); // Usunięcie hooka
        hook = nullptr;
        instance = nullptr;
        qDebug() << "[GlobalKeyListener] Hook zatrzymany.";
    }
}

void GlobalKeyListener::start()
{
    if (hook) {
        qDebug() << "[GlobalKeyListener] Hook już aktywny.";
        return;
    }

    if (!hook) {
        instance = this;

        // Ustawienie globalnego hooka klawiatury (typu low-level)
        hook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardProc, nullptr, 0);
        if (!hook) {
            qWarning() << "[GlobalKeyListener] Nie udało się ustawić hooka!";
        } else {
            qDebug() << "[GlobalKeyListener] Hook aktywowany.";
        }
    }
}

// Callback wywoływany przez system Windows, gdy naciśnięto klawisz
LRESULT CALLBACK GlobalKeyListener::keyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && wParam == WM_KEYDOWN && instance != nullptr) {
        KBDLLHOOKSTRUCT *p = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);
        DWORD vkCode = p->vkCode;

        QMetaObject::invokeMethod(
            instance, [vkCode]() { emit instance->keyPressed(vkCode); }, Qt::QueuedConnection);
    }

    return CallNextHookEx(hook, nCode, wParam, lParam);
}
