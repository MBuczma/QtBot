#include "GlowneOkno.h"
#include "ui_GlowneOkno.h"

GlowneOkno::GlowneOkno(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GlowneOkno)
{
    ui->setupUi(this);
}

GlowneOkno::~GlowneOkno()
{
    delete ui;
}
