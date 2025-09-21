/* OknoBot.h */
/*
 * Okno które przechowuje rzędy boxów z przyciskami do wysłania.
 */
#ifndef OKNOBOT_H
#define OKNOBOT_H

#include <QWidget>
#include "GlobalKeyListener.h"
#include "GlobalMouseListener.h"

class GroupBoxControl;
class GlobalMouseListener;

namespace Ui {
class OknoBot;
}

class OknoBot : public QWidget
{
    Q_OBJECT

public:
    explicit OknoBot(QWidget *parent = nullptr);
    ~OknoBot();
    QString getAllDataFromGroupBox();
    void usunWszystkieRzedy();
    void setAllDataToGroupBox(QString zawartoscPliku);

public slots:
    void pobierzIdWszystkie();

signals:
    void rozszerzOkno(short height);

private slots:
    void dodajRzad();
    void usunKonkretnegoGroupBoxa(GroupBoxControl *kto);
    void startWszystkie(); // Funkcję do zatrzymywania wszystkich
    void stopWszystkie(); // Funkcję do zatrzymywania wszystkich
    void onKeyPressed(WPARAM vkCode);

private:
    std::unique_ptr<Ui::OknoBot> ui;
    std::vector<GroupBoxControl *>
        groupBoxes; // Wektor przechowujący wskaźniki do dynamicznych GroupBoxControl
    GlobalKeyListener *globalKeyListener = nullptr;
    std::unique_ptr<GlobalMouseListener> globalMouseListener;
    bool isButtonPressedGlobal = false;
    HWND handleGlobal = NULL;
    HWND parentHandleGlobal = NULL;

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
};

#endif // OKNOBOT_H
