/* main.cpp */
#include "GlowneOkno.h"
#include <QApplication>
#include <QStyleFactory>
#include <memory>  // std::unique_ptr

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    std::unique_ptr<GlowneOkno> glowneOkno = std::make_unique<GlowneOkno>();
    glowneOkno->show();
    return application.exec();
}
