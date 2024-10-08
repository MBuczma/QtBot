/* NagrywanieOkno.h */
#ifndef NAGRYWANIEOKNO_H
#define NAGRYWANIEOKNO_H

#include "AutoKeyPresser.h"
#include <QWidget>

namespace Ui {
class NagrywanieOkno;
}

class NagrywanieOkno : public QWidget
{
    Q_OBJECT

public:
    explicit NagrywanieOkno(QWidget *parent = nullptr);
    ~NagrywanieOkno();

private slots:
    void ZlapIdOkna();
    void rozpocznijWysylanie();

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::NagrywanieOkno *ui;
    AutoKeyPresser *autoKeyPresser;
    HWND handle;
    HWND parentHandle;
    QString windowText;
    QString parentHandleWindowText;
    bool isButtonPressed;
};

#endif // NAGRYWANIEOKNO_H
