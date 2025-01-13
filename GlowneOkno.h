/* GlowneOkno.h */
/*
 * Obiekt odpowiedzialny za wyświetlanie głównego okna z menu.
 * Przyciski:
 * - Start
 * - Mouse Tracker
 * - Informacje
 * - Wyjście
 */
#ifndef GLOWNEOKNO_H
#define GLOWNEOKNO_H

#include <QMainWindow>

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

private slots:
    void start();
    void mouseTracker();
    void informacje();
    void wyjscie();

private:
    std::unique_ptr<Ui::GlowneOkno> ui; // Zarządzanie wskaźnikiem Ui
    std::unique_ptr<OknoBot> oknoBot;   // Zarządzanie wskaźnikiem OknoBot
    std::unique_ptr<NagrywanieOkno> nagrywanieOkno; // Zarządzanie wskaźnikiem NagrywanieOkno
    static short width;
    static short height;
};

#endif // GLOWNEOKNO_H
