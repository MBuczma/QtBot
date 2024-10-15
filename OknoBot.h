/* OknoBot.h */
#ifndef OKNOBOT_H
#define OKNOBOT_H

#include <QTimer>
#include <QWidget>
#include "AutoKeyPresser.h"
#include <memory>

namespace Ui {
class OknoBot;
}

class OknoBot : public QWidget
{
    Q_OBJECT

public:
    explicit OknoBot(QWidget *parent = nullptr);
    ~OknoBot();

signals:
    void rozszerzOkno(int height);

private slots:
    void startStop();
    void dodajRzad();
    void ZlapIdOkna();
    void zaktualizujNazwe();
    void aktualizujStanPrzyciskuStartNaPodstawieComboBox(const QString &text);

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    std::unique_ptr<Ui::OknoBot> ui;
    std::unique_ptr<AutoKeyPresser> autoKeyPresser;
    std::unique_ptr<QTimer> keyTimer;
    std::unique_ptr<QTimer> countdownTimer;

    HWND handle;
    HWND parentHandle;
    QString windowText;
    QString parentHandleWindowText;

    bool isButtonPressed = false; // Zmienna do przechowywania stanu przycisku
    bool isSendingKeys = false;   // Określa, czy klawisze są aktualnie wysyłane
    int remainingTime = 0;        // Przechowuje pozostały czas do wysłania klawisza

    void wyslijKlawisz();
    void aktualizujCountdown();
    void aktualizujStanPrzyciskuStart(bool isSending);
};

#endif // OKNOBOT_H
