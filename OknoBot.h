/* OknoBot.h */
/*
 * Okno które przechowuje rzędy boxów z przyciskami do wysłania.
 */
#ifndef OKNOBOT_H
#define OKNOBOT_H

#include <QWidget>

class AutoKeyPresser;
class GroupBoxControl;

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

signals:
    void rozszerzOkno(short height);

private slots:
    void dodajRzad();
    void usunKonkretnegoGroupBoxa(GroupBoxControl *kto);
    void startWszystkie(); // Funkcję do zatrzymywania wszystkich
    void stopWszystkie(); // Funkcję do zatrzymywania wszystkich

private:
    std::unique_ptr<Ui::OknoBot> ui;
    std::vector<GroupBoxControl *>
        groupBoxes; // Wektor przechowujący wskaźniki do dynamicznych GroupBoxControl
};

#endif // OKNOBOT_H
