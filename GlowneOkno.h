/* GlowneOkno.h */
#ifndef GLOWNEOKNO_H
#define GLOWNEOKNO_H

#include <QMainWindow>
#include "NagrywanieOkno.h"
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
    void mouseTracker();
    void informacje();
    void wyjscie();

private:
    Ui::GlowneOkno *ui;
    OknoBot *oknoBot;
    NagrywanieOkno *nagrywanieOkno;
};
#endif // GLOWNEOKNO_H
