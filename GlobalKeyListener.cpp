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
    // Sprawdzamy czy zdarzenie klawisza jest istotne i czy mamy aktywną instancję
    if (nCode == HC_ACTION && wParam == WM_KEYDOWN && instance) {
        // Struktura z informacjami o klawiszu
        KBDLLHOOKSTRUCT *p = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);
        DWORD vkCode = p->vkCode;

        // Wywołujemy kod Qt w kolejce eventów (bo hook działa w innym wątku)
        QMetaObject::invokeMethod(
            instance,
            [vkCode]() {
                // Opis klawisza w formie tekstu
                QString keyText = KeyMap::getKeyText(vkCode);
                qDebug() << "[GlobalKeyListener] Wciśnięto VK:" << vkCode
                         << (keyText.isEmpty() ? "" : QString("(%1)").arg(keyText));

                // Emisja sygnału do OknoBot
                emit instance->keyPressed(vkCode);
            },
            Qt::QueuedConnection); // gwarantuje wykonanie w głównym wątku Qt
    }

    // Przekazanie zdarzenia dalej do systemu
    return CallNextHookEx(hook, nCode, wParam, lParam);
}
