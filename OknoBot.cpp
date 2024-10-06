/* OknoBot.cpp */
#include "OknoBot.h"
#include "ui_OknoBot.h"
#include <QMouseEvent>

OknoBot::OknoBot(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OknoBot)
    , autoKeyPresser(new AutoKeyPresser(this))
    , handle()
    , parentHandle()
    , isButtonPressed(false)
{
    ui->setupUi(this);
    connect(ui->pushButton_Start, &QPushButton::clicked, this, &OknoBot::start);
    connect(ui->pushButton_DodajNowyRzad, &QPushButton::clicked, this, &OknoBot::dodajRzadPrzyciskow);
    connect(ui->pushButton_PobierzID, &QPushButton::pressed, this, &OknoBot::ZlapIdOkna);
}

OknoBot::~OknoBot()
{
    delete autoKeyPresser;
    delete ui;
}

void OknoBot::start()
{
    qDebug() << "Przycisk start z oknobot został naciśnięty.";
}

void OknoBot::dodajRzadPrzyciskow()
{
    qDebug() << "Przycisk dodajRzadPrzyciskow został naciśnięty.";
}

void OknoBot::ZlapIdOkna()
{
    qDebug() << "\n" << "Metoda OknoBot::ZlapIdOkna() został wywołana.";
    isButtonPressed = true;
    grabMouse();
    setCursor(Qt::CrossCursor);
}

void OknoBot::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "OknoBot::mouseReleaseEvent.";
    if (event->button() == Qt::LeftButton && isButtonPressed == true){
        isButtonPressed = false;
        autoKeyPresser->ZlapIdOkna(handle, parentHandle);
        releaseMouse();
        unsetCursor();
        QString windowText = autoKeyPresser->GetWindowTextFromHandle(handle);
        ui->lineEdit_NazwaProgramu->setText(windowText);
        QString parentHandleText = autoKeyPresser->GetWindowTextFromHandle(parentHandle);
        ui->groupBoxPrzyciski->setTitle(parentHandleText);

    }
}
