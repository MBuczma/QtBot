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
    void zaktualizujNazwe();


protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::OknoBot *ui;
    AutoKeyPresser *autoKeyPresser;
    HWND handle;
    HWND parentHandle;
    QString windowText;
    QString parentHandleWindowText;
    QTimer *keyTimer;  // Timer do wysyłania klawisza
    bool isButtonPressed;
    void wyslijKlawisz();
    QTimer *countdownTimer;  // Timer do odliczania czasu do wysyłki
    int remainingTime;  // Zmienna przechowująca pozostały czas do wysłania
    void aktualizujCountdown();  // Funkcja do aktualizowania odliczania
    bool isSendingKeys;  // Zmienna określająca, czy klawisze są wysyłane
};
#endif // OKNOBOT_H
