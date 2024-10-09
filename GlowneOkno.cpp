/* GlowneOkno.cpp */
#include "GlowneOkno.h"
#include "ui_GlowneOkno.h"
#include "NagrywanieOkno.h"

GlowneOkno::GlowneOkno(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::GlowneOkno>())
    , oknoBot(std::make_unique<OknoBot>(this))
    , nagrywanieOkno(std::make_unique<NagrywanieOkno>(this))
{
    ui->setupUi(this);
    ui->stackedWidget->addWidget(oknoBot.get());
    ui->stackedWidget->addWidget(nagrywanieOkno.get());

    // Łączenie sygnałów z przyciskami
    connect(ui->PrzyciskStart, &QPushButton::clicked, this, &GlowneOkno::start);
    connect(ui->PrzyciskMouseTracker, &QPushButton::clicked, this, &GlowneOkno::mouseTracker);
    connect(ui->PrzyciskInfo, &QPushButton::clicked, this, &GlowneOkno::informacje);
    connect(ui->PrzyciskWyjscie, &QPushButton::clicked, this, &GlowneOkno::wyjscie);
    connect(ui->actionWyjdz, &QAction::triggered, QApplication::instance(), &QApplication::quit);
}

GlowneOkno::~GlowneOkno() = default; // Destruktor domyślny

void GlowneOkno::start()
{
    qDebug() << "Przycisk start został naciśnięty.";
    ui->stackedWidget->setCurrentWidget(oknoBot.get());
    this->adjustSize();
    this->setMinimumSize(650, 250);
    this->setMaximumSize(650, 1000);
    this->setFocus();
}

void GlowneOkno::mouseTracker()
{
    qDebug() << "Przycisk mouseTracker został naciśnięty.";
    ui->stackedWidget->setCurrentWidget(nagrywanieOkno.get());
    this->adjustSize();
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
