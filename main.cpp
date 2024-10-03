#include "GlowneOkno.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GlowneOkno w;
    w.show();
    return a.exec();
}
