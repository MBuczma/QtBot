/* OknoBot.cpp */
#include "OknoBot.h"
#include "ui_OknoBot.h"
#include <QMouseEvent>

OknoBot::OknoBot(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OknoBot)
    , autoKeyPresser(new AutoKeyPresser(this))
    , handle(0)
    , parentHandle(0)
    , windowText("windowText")
    , parentHandleWindowText("parentHandleWindowText")
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
    //ui->pushButton_PobierzID->
}

void OknoBot::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && isButtonPressed == true){
        qDebug() << "OknoBot::mouseReleaseEvent.";
        isButtonPressed = false;
        autoKeyPresser->ZlapIdOkna(handle, parentHandle);
        releaseMouse();
        unsetCursor();
        zaktualizujNazwe();
        ui->pushButton_PobierzID->setDown(false);
    }
}

void OknoBot::zaktualizujNazwe()
{
    windowText = autoKeyPresser->GetWindowTextFromHandle(handle);
    ui->lineEdit_NazwaProgramu->setText(windowText);
    parentHandleWindowText = autoKeyPresser->GetWindowTextFromHandle(parentHandle);
    ui->groupBoxPrzyciski->setTitle(parentHandleWindowText);
}
