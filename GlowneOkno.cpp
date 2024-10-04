/* GlowneOkno.cpp */
#include "GlowneOkno.h"
#include "ui_GlowneOkno.h"

GlowneOkno::GlowneOkno(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GlowneOkno)
    , oknoBot(new OknoBot(this))
{
    ui->setupUi(this);
    ui->stackedWidget->addWidget(oknoBot);
    connect(ui->PrzyciskStart, &QPushButton::clicked, this, &GlowneOkno::start);
    connect(ui->PrzyciskInfo, &QPushButton::clicked, this, &GlowneOkno::informacje);
    connect(ui->PrzyciskWyjscie, &QPushButton::clicked, this, &GlowneOkno::wyjscieZProgramu);
}

GlowneOkno::~GlowneOkno()
{
    delete ui;
}

void GlowneOkno::start()
{
    qDebug() << "Przycisk start został naciśnięty.";
    ui->stackedWidget->setCurrentWidget(oknoBot);
}

void GlowneOkno::informacje()
{
    qDebug() << "Przycisk informacje został naciśnięty.";
}

void GlowneOkno::wyjscieZProgramu()
{
    qDebug() << "Przycisk wyjscie został naciśnięty.";
    QApplication::quit();
}
