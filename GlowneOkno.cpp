/* GlowneOkno.cpp */
#include "GlowneOkno.h"
#include "ui_GlowneOkno.h"

GlowneOkno::GlowneOkno(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GlowneOkno)
{
    ui->setupUi(this);
    connect(ui->PrzyciskWyjscie, &QPushButton::clicked, this, &GlowneOkno::wyjscieZProgramu);
}

GlowneOkno::~GlowneOkno()
{
    delete ui;
}

void GlowneOkno::wyjscieZProgramu()
{
    qDebug() << "Przycisk wyjscie został naciśnięty.";
    QApplication::quit();
}
