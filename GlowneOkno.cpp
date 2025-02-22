/* GlowneOkno.cpp */
/*
 * Obiekt odpowiedzialny za wyświetlanie głównego okna z menu.
 * Przyciski:
 * - Start
 * - Informacje
 * - Wyjście
 */

#include "GlowneOkno.h"
#include "ui_GlowneOkno.h"

#include <QMessageBox>
#include "Logger.h"
#include "OknoBot.h"
#include <memory> // std::unique_ptr

short GlowneOkno::width = 570;
short GlowneOkno::height = 180;

GlowneOkno::GlowneOkno(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::GlowneOkno>())
    , oknoBot(std::make_unique<OknoBot>(this))
{
    ui->setupUi(this);
    ui->stackedWidget->addWidget(oknoBot.get());

    // Łączenie sygnałów z przyciskami na ekranie
    connect(ui->PrzyciskStart, &QPushButton::clicked, this, &GlowneOkno::start);
    connect(ui->PrzyciskInfo, &QPushButton::clicked, this, &GlowneOkno::informacje);
    connect(ui->PrzyciskWyjscie, &QPushButton::clicked, this, &GlowneOkno::wyjscie);

    // Łączenie sygnałów z przyciskami w rozwijanym menu
    //connect(ui->actionZapisz, &QAction::triggered, oknoBot.get(), &OknoBot::startWszystkie);
    //connect(ui->actionWczytaj, &QAction::triggered, oknoBot.get(), &OknoBot::stopWszystkie);
    connect(ui->actionWyjdz, &QAction::triggered, QApplication::instance(), &QApplication::quit);

    // Połączenie przycisku z lambda (funkcja anonimowa)
    connect(oknoBot.get(), &OknoBot::rozszerzOkno, this, [this](short incrementHeight) {
        GlowneOkno::height += incrementHeight;
        this->setMinimumSize(width, height);
        this->setMaximumSize(width, height);
    });
}

GlowneOkno::~GlowneOkno() = default; // Destruktor domyślny

void GlowneOkno::start()
{
    qDebug() << "Przycisk start został naciśnięty.";
    ui->stackedWidget->setCurrentWidget(oknoBot.get());
    this->setMinimumSize(width, height);
    this->setMaximumSize(width, height);
}

void GlowneOkno::informacje()
{
    qDebug() << "Przycisk informacje został naciśnięty.";
}

void GlowneOkno::wyjscie()
{
    qDebug() << "Przycisk wyjscie został naciśnięty.";
    QApplication::quit();
}

void GlowneOkno::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                              "Zamykanie",
                                                              "Na pewno chcesz wyjść z programu?",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Użytkownik potwierdził – zamykamy logger i akceptujemy zamknięcie okna
        closeLogger(); // <-- zamknie plik, przywróci oryginalny handler
        event->accept();
    } else {
        event->ignore();
    }
}
