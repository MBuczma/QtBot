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
#include <exception>

void ustawLoggera();
void ustawStyl(QApplication &application);

int main(int argc, char *argv[])
{
    try {
        QApplication application(argc, argv);

        ustawLoggera();
        qInfo() << "[main] Logger zainicjalizowany.";

        ustawStyl(application);
        qInfo() << "[main] Styl graficzny ustawiony.";

        std::unique_ptr<GlowneOkno> glowneOkno = std::make_unique<GlowneOkno>();
        qInfo() << "[main] Główne okno utworzone.";

        glowneOkno->show();
        qInfo() << "[main] Wyświetlono główne okno.";

        int result = application.exec();
        qInfo() << "[main] Pętla aplikacji zakończona, kod wyjścia:" << result;

        closeLogger();
        return result;

    } catch (const std::exception &e) {
        qCritical() << "[main] Wyjątek std::exception:" << e.what();
    } catch (...) {
        qCritical() << "[main] Nieznany wyjątek podczas uruchamiania aplikacji.";
    }

    closeLogger();
    return -1;
}

void ustawLoggera()
{
    QCoreApplication::setApplicationName("QtBot");
    QString logDir = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)
                     + "/Logs";
    if (!QDir().mkpath(logDir)) {
        qWarning() << "[ustawLoggera] Nie udało się utworzyć katalogu logów:" << logDir;
    }

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");
    QString logFile = logDir + "/QtBot_log_" + timestamp + ".txt";

    initLogger(logFile);
    qInfo() << "[ustawLoggera] Logi zapisywane w:" << logFile;
}

void ustawStyl(QApplication &application)
{
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QPalette palette = application.palette();
    application.setStyleSheet("QPushButton:hover { background-color: #808080; }"
                              "QComboBox:hover { background-color: #808080; }");
    application.setPalette(palette);
}
