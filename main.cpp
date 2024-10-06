/* main.cpp */
#include "GlowneOkno.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    GlowneOkno glowneOkno;
    glowneOkno.show();
    return application.exec();
}
