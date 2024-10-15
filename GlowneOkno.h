/* GlowneOkno.h */
#ifndef GLOWNEOKNO_H
#define GLOWNEOKNO_H

#include <QMainWindow>
#include <memory> // std::unique_ptr

class OknoBot;        // Deklaracja wstępna klasy OknoBot
class NagrywanieOkno; // Deklaracja wstępna klasy NagrywanieOkno

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

    static short width;
    static short height;

private slots:
    void start();
    void mouseTracker();
    void informacje();
    void wyjscie();

private:
    std::unique_ptr<Ui::GlowneOkno> ui; // Zarządzanie wskaźnikiem Ui
    std::unique_ptr<OknoBot> oknoBot;   // Zarządzanie wskaźnikiem OknoBot
    std::unique_ptr<NagrywanieOkno> nagrywanieOkno; // Zarządzanie wskaźnikiem NagrywanieOkno
};

#endif // GLOWNEOKNO_H
