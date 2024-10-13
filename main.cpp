/* main.cpp */
#include <QApplication>
#include <QPalette>
#include <QStyleFactory>
#include "GlowneOkno.h"
#include <memory> // std::unique_ptr

void ustawStyl(QApplication &application);

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    ustawStyl(application);

    std::unique_ptr<GlowneOkno> glowneOkno = std::make_unique<GlowneOkno>();
    glowneOkno->show();

    return application.exec();
}

void ustawStyl(QApplication &application)
{
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QPalette palette = application.palette();
    application.setStyleSheet("QPushButton:hover { background-color: #808080; }"
                              "QComboBox:hover { background-color: #808080; }");
    application.setPalette(palette);
}
