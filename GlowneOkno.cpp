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
#include "ui_GlowneOkno.h"

#include <QFileDialog>
#include <QMessageBox>
#include "OknoBot.h"

short GlowneOkno::width = 830;
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

    connect(ui->PrzyciskWroc, &QPushButton::clicked, this, [this]() {
        ui->stackedWidget->setCurrentWidget(ui->page_Start);
    });

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
}

GlowneOkno::~GlowneOkno() = default; // Destruktor domyślny

void GlowneOkno::start()
{
    qDebug() << "[GlowneOkno] Przycisk start() został naciśnięty.";
    ui->stackedWidget->setCurrentWidget(oknoBot.get());
    this->setMinimumSize(width, height);
    this->setMaximumSize(width, height);
}

void GlowneOkno::informacje()
{
    qDebug() << "[GlowneOkno] Przycisk informacje() został naciśnięty.";
    ui->stackedWidget->setCurrentWidget(ui->page_info);
}

void GlowneOkno::wyjscie()
{
    qDebug() << "[GlowneOkno] Przycisk wyjscie() został naciśnięty.";
    QApplication::quit();
}

/*
* Sekcja przycisków z Menu belki
*/
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
        qInfo() << "[GlowneOkno][zapiszPlik] Zapis pliku anulowany przez użytkownika.";
        return;
    }

    QFile plik(sciezkaPliku);
    if (!plik.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Błąd"), tr("Nie można otworzyć pliku do zapisu."));
        qWarning() << "[GlowneOkno][zapiszPlik] Nie można otworzyć pliku do zapisu:"
                   << sciezkaPliku;
        return;
    }

    QTextStream out(&plik);
    out << oknoBot->getAllDataFromGroupBox();
    plik.close();
    qInfo() << "[GlowneOkno][zapiszPlik] Zapisano dane do pliku:" << sciezkaPliku;
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
        qInfo() << "[GlowneOkno][wczytajPlik] Wczytanie pliku anulowane przez użytkownika.";
        return;
    }

    QFile plik(sciezkaPliku);
    if (!plik.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Błąd"), tr("Nie można otworzyć pliku do odczytu."));
        qWarning() << "[GlowneOkno][wczytajPlik] Nie można otworzyć pliku do odczytu:"
                   << sciezkaPliku;
        return;
    }

    QTextStream in(&plik);
    QString zawartoscPliku = in.readAll();
    int liczbaLinii = zawartoscPliku.split('\n', Qt::SkipEmptyParts).size();
    qInfo() << "[GlowneOkno][wczytajPlik] Wczytano dane z pliku:" << sciezkaPliku
            << "Liczba linii:" << liczbaLinii;
    qDebug().noquote() << zawartoscPliku;

    if (ui->stackedWidget->currentWidget() != oknoBot.get()) {
        ui->stackedWidget->setCurrentWidget(oknoBot.get());
        qInfo() << "[GlowneOkno][wczytajPlik] Po wczytaniu profilu przełączono na widok OknoBot.";
    }
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
        qInfo() << Qt::endl
                << "[GlowneOkno][closeEvent] Użytkownik potwierdził zamknięcie programu.";
        event->accept();
    } else {
        qInfo() << "[GlowneOkno][closeEvent] Użytkownik anulował zamknięcie programu.";
        event->ignore();
    }
}
