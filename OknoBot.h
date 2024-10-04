#ifndef OKNOBOT_H
#define OKNOBOT_H

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

private:
    Ui::OknoBot *ui;
};

#endif // OKNOBOT_H
