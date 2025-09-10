#include "GlobalMouseListener.h"
#include <QDebug>
#include <QMetaObject>

HHOOK GlobalMouseListener::hook = nullptr;
GlobalMouseListener *GlobalMouseListener::instance = nullptr;

GlobalMouseListener::GlobalMouseListener(QObject *parent)
    : QObject(parent)
{}

GlobalMouseListener::~GlobalMouseListener()
{
    if (hook) {
        UnhookWindowsHookEx(hook);
        hook = nullptr;
        instance = nullptr;
        qDebug() << "[GlobalMouseListener] Hook zatrzymany.";
    }
}

void GlobalMouseListener::start()
{
    if (hook) {
        qDebug() << "[GlobalMouseListener] Hook już aktywny.";
        return;
    }
    instance = this;
    hook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, nullptr, 0);
    if (!hook)
        qWarning() << "[GlobalMouseListener] Nie udało się ustawić hooka!";
    else
        qDebug() << "[GlobalMouseListener] Hook aktywowany.";
}

LRESULT CALLBACK GlobalMouseListener::mouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION && instance) {
        const MSLLHOOKSTRUCT *p = reinterpret_cast<MSLLHOOKSTRUCT *>(lParam);

        if (wParam == WM_XBUTTONDOWN || wParam == WM_XBUTTONUP) {
            const WORD hi = HIWORD(p->mouseData); // 1 = XBUTTON1, 2 = XBUTTON2
            int button = (hi == XBUTTON1) ? 1 : (hi == XBUTTON2 ? 2 : 0);
            if (button != 0) {
                bool down = (wParam == WM_XBUTTONDOWN);
                QMetaObject::invokeMethod(
                    instance,
                    [button, down]() { emit instance->xButton(button, down); },
                    Qt::QueuedConnection);
            }
        }
    }
    return CallNextHookEx(hook, nCode, wParam, lParam);
}
