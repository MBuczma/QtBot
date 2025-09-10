#ifndef GLOBALMOUSELISTENER_H
#define GLOBALMOUSELISTENER_H

#include <QObject>
#include <Windows.h>

class GlobalMouseListener : public QObject
{
    Q_OBJECT
signals:
    // button: 1 = XBUTTON1, 2 = XBUTTON2; down = true (DOWN) / false (UP)
    void xButton(int button, bool down);

public:
    explicit GlobalMouseListener(QObject *parent = nullptr);
    ~GlobalMouseListener();
    void start();

private:
    static LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static HHOOK hook;
    static GlobalMouseListener *instance;
};

#endif
