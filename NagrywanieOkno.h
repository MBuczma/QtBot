/* NagrywanieOkno.h */
#ifndef NAGRYWANIEOKNO_H
#define NAGRYWANIEOKNO_H

#include <QWidget>
#include "AutoKeyPresser.h"
#include <memory>
#include <optional>

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

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    std::unique_ptr<Ui::NagrywanieOkno> ui;
    std::unique_ptr<AutoKeyPresser> autoKeyPresser;

    std::optional<HWND> handle;       // Uchwyty okien systemu Windows
    std::optional<HWND> parentHandle; // Uchwyty okien rodzica

    QString windowText;
    QString parentHandleWindowText;
    bool isButtonPressed = false; // Przechowuje stan przycisku
};

#endif // NAGRYWANIEOKNO_H
