/* main.cpp */
/*
 * Główny plik w którym jest tworzony obiekt QApplication o nazwie application.
 * Tworzy aplikację Qt i uruchamia główne okno (GlowneOkno).
 * W funkcji ustawStyl jest ustawiany wygląd GUI poprzez setStyle i setPalette.
 * Inicjalizuje logger.
 */

//Opisy dla użytkownika
//Komentarze dla siebie
//Ujednolicić nazwy funkcji i zmiennych

#include "GlowneOkno.h"
#include "Logger.h"

#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QPalette>
#include <QStandardPaths>
#include <QStyleFactory>
#include <memory> // std::unique_ptr

void ustawLoggera();
void ustawStyl(QApplication &application);

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    ustawLoggera();
    ustawStyl(application);

    std::unique_ptr<GlowneOkno> glowneOkno = std::make_unique<GlowneOkno>();
    glowneOkno->show();

    return application.exec();
}

void ustawLoggera()
{
    QCoreApplication::setApplicationName("QtBot");
    QString logDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
                     + "/Logs";
    QDir().mkpath(logDir);

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");
    QString logFile = logDir + "/QtBot_log_" + timestamp + ".txt";

    initLogger(logFile);
    qInfo() << "log_file to " << logFile;
    qInfo() << "Uruchomiono aplikacje";
}

void ustawStyl(QApplication &application)
{
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QPalette palette = application.palette();
    application.setStyleSheet("QPushButton:hover { background-color: #808080; }"
                              "QComboBox:hover { background-color: #808080; }");
    application.setPalette(palette);
}
