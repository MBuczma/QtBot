#include "mainwindow.h"
#include "qnamespace.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <cstddef>
#include <utilapiset.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void sleepSec()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


void MainWindow::on_Start1_clicked()
{
    int nastawionyCzas = ui->ZadanyCzas1->value();
    if(ui->Timer1->value()==0)
    {
        MainWindow::reset1 = false;
        ui->Timer1->setValue(nastawionyCzas);
        Timer1_valueChanged(nastawionyCzas);
    }

}


void MainWindow::on_Reset1_clicked()
{
    ui->Timer1->setValue(NULL);
    MainWindow::reset1 = true;
}


void MainWindow::Timer1_valueChanged(int arg1)
{
    static int czasDoAkcji = arg1;
    do
    {
        if (MainWindow::reset1 == false)
        {
            if (czasDoAkcji > 0)
            {
               sleepSec();
               czasDoAkcji--;
               ui->Timer1->setValue(czasDoAkcji);
            }
            else
            {
                Beep(523,200); // 523 hertz (C5) for 500 milliseconds
                czasDoAkcji = arg1;
            }
         }
            else {
                ui->Timer1->setValue(NULL);
                break;
            }
    }while(true);
}

