/* main.cpp */
#include "GlowneOkno.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    GlowneOkno window;
    window.show();
    return application.exec();
}
