/* GlowneOkno.cpp */
#include "GlowneOkno.h"
#include "ui_GlowneOkno.h"
#include "NagrywanieOkno.h"

GlowneOkno::GlowneOkno(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GlowneOkno)
    , oknoBot(new OknoBot(this))
    , nagrywanieOkno(new NagrywanieOkno(this))
{
    ui->setupUi(this);
    ui->stackedWidget->addWidget(oknoBot);
    ui->stackedWidget->addWidget(nagrywanieOkno);
    connect(ui->PrzyciskStart, &QPushButton::clicked, this, &GlowneOkno::start);
    connect(ui->PrzyciskMouseTracker, &QPushButton::clicked, this, &GlowneOkno::mouseTracker);
    connect(ui->PrzyciskInfo, &QPushButton::clicked, this, &GlowneOkno::informacje);
    connect(ui->PrzyciskWyjscie, &QPushButton::clicked, this, &GlowneOkno::wyjscie);
}

GlowneOkno::~GlowneOkno()
{
    delete oknoBot;
    delete ui;
}

void GlowneOkno::start()
{
    qDebug() << "Przycisk start został naciśnięty.";
    ui->stackedWidget->setCurrentWidget(oknoBot);
}

void GlowneOkno::mouseTracker()
{
    qDebug() << "Przycisk mouseTracker został naciśnięty.";
    ui->stackedWidget->setCurrentWidget(nagrywanieOkno);
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
