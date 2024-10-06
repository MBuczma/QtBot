/* OknoBot.h */
#ifndef OKNOBOT_H
#define OKNOBOT_H

#include "AutoKeyPresser.h"
#include <QWidget>

namespace Ui {
class OknoBot;
}

class OknoBot : public QWidget
{
    Q_OBJECT

public:
    explicit OknoBot(QWidget *parent = nullptr);
    ~OknoBot();

private slots:
    void start();
    void dodajRzadPrzyciskow();
    void ZlapIdOkna();

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::OknoBot *ui;
    AutoKeyPresser *autoKeyPresser;
    HWND handle;
    HWND parentHandle;
    bool isButtonPressed;
};
#endif // OKNOBOT_H