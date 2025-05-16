#ifndef GLOBALKEYLISTENER_H
#define GLOBALKEYLISTENER_H

#include <QObject>
#include <Windows.h>

class GlobalKeyListener : public QObject
{
    Q_OBJECT

public:
    explicit GlobalKeyListener(QObject *parent = nullptr);
    ~GlobalKeyListener();

    void start();
    void stop();

private:
    static LRESULT CALLBACK keyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static HHOOK hook;
    static GlobalKeyListener *instance;

    static QString opisVK(int vkCode); // dodane
};

#endif // GLOBALKEYLISTENER_H
