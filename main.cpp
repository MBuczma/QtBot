#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication aplikacja(argc, argv);
    MainWindow glowneOkno ;
    glowneOkno.show();


    return aplikacja.exec();
}





