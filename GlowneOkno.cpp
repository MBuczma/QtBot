/* GlowneOkno.cpp */
/*
 * Obiekt odpowiedzialny za wyświetlanie głównego okna z menu.
 * Przyciski:
 * - Start
 * - Informacje
 * - Wyjście
 * Inicjuje OknoBot, które zawiera wszystkie wiersze (GroupBoxControl).
 * Obsługuje zamknięcie aplikacji i zapis ustawień.
 */

#include "GlowneOkno.h"
#include "GlobalKeyListener.h"
#include "ui_GlowneOkno.h"

#include <QFileDialog>
#include <QMessageBox>
#include "Logger.h"
#include "OknoBot.h"
#include <memory> // std::unique_ptr

short GlowneOkno::width = 670;
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
    connect(ui->actionZapisz, &QAction::triggered, this, &GlowneOkno::zapiszPlik);
    connect(ui->actionWczytaj, &QAction::triggered, this, &GlowneOkno::wczytajPlik);
    connect(ui->actionWyjdz, &QAction::triggered, QApplication::instance(), &QApplication::quit);

    // Połączenie przycisku z lambda (funkcja anonimowa)
    connect(oknoBot.get(), &OknoBot::rozszerzOkno, this, [this](short incrementHeight) {
        GlowneOkno::height += incrementHeight;
        this->setMinimumSize(width, height);
        this->setMaximumSize(width, height);
    });

    auto listener = new GlobalKeyListener(this);
    listener->start();
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

void GlowneOkno::zapiszPlik()
{
    QString domyslnaNazwaPliku = "ProfileSave.QtBP";
    QString sciezkaPliku
        = QFileDialog::getSaveFileName(this,
                                       tr("Zapisz plik jako"), // Tytuł okna dialogowego
                                       QDir::homePath() + "/"
                                           + domyslnaNazwaPliku, // Domyślna ścieżka i nazwa pliku
                                       tr("Pliki QtBot profile (*.QtBP)") // Filtry typów plików
        );

    if (sciezkaPliku.isEmpty()) {
        // Użytkownik anulował operację
        return;
    }

    QFile plik(sciezkaPliku);
    if (!plik.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Błąd"), tr("Nie można otworzyć pliku do zapisu."));
        return;
    }

    oknoBot->getAllDataFromGroupBox();

    QTextStream out(&plik);
    //out << "Przykładowa zawartość pliku.\n";
    out << oknoBot->getAllDataFromGroupBox();

    plik.close();
}

void GlowneOkno::wczytajPlik()
{
    QString sciezkaPliku
        = QFileDialog::getOpenFileName(this, // Wskaźnik na rodzica, np. QWidget lub QMainWindow
                                       tr("Otwórz plik"),                 // Tytuł okna dialogowego
                                       QDir::homePath(),                  // Domyślna ścieżka
                                       tr("Pliki QtBot profile (*.QtBP)") // Filtry typów plików
        );

    if (sciezkaPliku.isEmpty()) {
        // Użytkownik anulował operację
        return;
    }

    QFile plik(sciezkaPliku);
    if (!plik.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Błąd"), tr("Nie można otworzyć pliku do odczytu."));
        return;
    }

    QTextStream in(&plik);
    QString zawartoscPliku = in.readAll();

    qDebug() << zawartoscPliku;
    oknoBot->usunWszystkieRzedy();
    oknoBot->setAllDataToGroupBox(zawartoscPliku);

    plik.close();
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
