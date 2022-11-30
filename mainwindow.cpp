#include "mainwindow.h"
#include "qnamespace.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <cstddef>
#include <utilapiset.h>
#include <QTimer>
#include <windows.h>

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
    QEventLoop loop;
    QTimer t;
    t.connect(&t, &QTimer::timeout, &loop, &QEventLoop::quit);
    t.start(1000);
    loop.exec();
}


void MainWindow::on_Start1_clicked()
{
    int nastawionyCzas = ui->ZadanyCzas1->value();
    if(ui->Timer1->value()==0 && nastawionyCzas!=0)
    {
        MainWindow::reset1 = false;
        ui->Timer1->setValue(nastawionyCzas);
        Timer1_valueChanged(nastawionyCzas);
    }

}

QString MainWindow::getPrzycisk1()
{
    return ui->Przycisk1->selectedText();
}

void pressKey(unsigned short virtual_key_code)
{
    // This structure will be used to create the keyboard
    // input event.
    INPUT ip;
    // Set up a generic keyboard event.

    ip.type = INPUT_KEYBOARD;
    // Press the "A" key
    //auto virtual_key_code = 0x7B;
    ip.ki.wVk = virtual_key_code; // virtual-key code for the "f10" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "A" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
}


void MainWindow::on_Reset1_clicked()
{
    ui->Timer1->setValue(NULL);
    MainWindow::reset1 = true;
}


void MainWindow::Timer1_valueChanged(int arg1)
{
    int czasDoAkcji = arg1;
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
                QString przycisk1 = ui->Przycisk1->text();
                Sleep(1000);
                pressKey(VK_F11); //0x7A 	F11 key
                Sleep(1000);
                pressKey(VK_DOWN); //0x28 	DOWN ARROW key
                Sleep(1000);
                pressKey(VK_UP); //0x26 	UP ARROW key
                Sleep(1000);
                pressKey(VK_F10); //0x79 	F10 key
                Sleep(1000);
                pressKey(VK_F10); //0x79 	F10 key
                if(arg1!=0)
                    czasDoAkcji = arg1;
                else
                    break;
            }
         }
            else {
                ui->Timer1->setValue(NULL);
                break;
            }
    }while(true);
}

