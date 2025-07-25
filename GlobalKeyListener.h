#ifndef GLOBALKEYLISTENER_H
#define GLOBALKEYLISTENER_H

#include <QObject>
#include <Windows.h>

class GlobalKeyListener : public QObject
{
    Q_OBJECT

signals:
    void keyPressed(WPARAM vkCode);

public:
    explicit GlobalKeyListener(QObject *parent = nullptr);
    ~GlobalKeyListener();

    void start();

private:
    static LRESULT CALLBACK keyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static HHOOK hook;
    static GlobalKeyListener *instance;
};

#endif // GLOBALKEYLISTENER_H
