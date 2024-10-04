/* GlowneOkno.h */
#ifndef GLOWNEOKNO_H
#define GLOWNEOKNO_H

#include <QMainWindow>
#include "OknoBot.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class GlowneOkno;
}
QT_END_NAMESPACE

class GlowneOkno : public QMainWindow
{
    Q_OBJECT

public:
    GlowneOkno(QWidget *parent = nullptr);
    ~GlowneOkno();

private slots:
    void start();
    void informacje();
    void wyjscieZProgramu();

private:
    Ui::GlowneOkno *ui;
    OknoBot *oknoBot;
};
#endif // GLOWNEOKNO_H
