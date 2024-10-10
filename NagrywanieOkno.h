/* NagrywanieOkno.h */
#ifndef NAGRYWANIEOKNO_H
#define NAGRYWANIEOKNO_H

#include <QWidget>
#include "AutoKeyPresser.h"
#include <memory>

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
    void ZlapIdOkna();          // Metoda odpowiedzialna za złapanie uchwytu okna
    void rozpocznijWysylanie(); // Rozpocznij wysyłanie klawiszy
    void zaktualizujNazwe();

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    std::unique_ptr<Ui::NagrywanieOkno> ui;
    std::unique_ptr<AutoKeyPresser> autoKeyPresser;

    HWND handle;
    HWND parentHandle;

    QString windowText;
    QString parentHandleWindowText;
    bool isButtonPressed = false; // Przechowuje stan przycisku
};

#endif // NAGRYWANIEOKNO_H
