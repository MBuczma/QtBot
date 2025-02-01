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

signals:
    void rozszerzOkno(short height);

private slots:
    void dodajRzad();

protected:
private:
    std::unique_ptr<Ui::OknoBot> ui;
};

#endif // OKNOBOT_H
