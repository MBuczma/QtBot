/**
 * @file main.cpp
 * @brief Główny plik programu QtBot.
 *
 * Tworzy obiekt QApplication, inicjalizuje logger oraz uruchamia główne okno aplikacji (GlowneOkno).
 * W funkcji `ustawStyl` ustawiany jest wygląd GUI poprzez `setStyle` i `setPalette`.
 * Program loguje kluczowe etapy uruchamiania i zamykania aplikacji.
 */

//Opisy dla użytkownika
//Komentarze dla siebie
//Ujednolicić nazwy funkcji i zmiennych

#include "GlowneOkno.h"
#include "Logger.h"

#include <QApplication>
#include <QDateTime>
#include <QDir>
#include <QIcon>
#include <QPalette>
#include <QStandardPaths>
#include <QStyleFactory>

void ustawLoggera();
void ustawStyl(QApplication &application);

/**
 * @brief Punkt wejścia aplikacji QtBot.
 *
 * Tworzy instancję aplikacji, inicjalizuje logger, ustawia styl oraz uruchamia główne okno.
 * Obsługuje wyjątki i zapisuje szczegóły do logów.
 *
 * @param[in] argc Liczba argumentów wiersza poleceń.
 * @param[in] argv Tablica argumentów wiersza poleceń.
 * @return Kod wyjścia programu (0 w przypadku poprawnego zakończenia, -1 w razie błędu).
 */
int main(int argc, char *argv[])
{
    try {
        QApplication application(argc, argv);
        application.setWindowIcon(QIcon(":/QTBot_robot_icon.ico"));

        ustawLoggera();
        qInfo() << "[main] Logger zainicjalizowany.";

        qInfo() << "[main] Wersja programu: Beta 1.0.0";

        ustawStyl(application);
        qInfo() << "[main] Styl graficzny ustawiony.";

        GlowneOkno glowneOkno;
        qInfo() << "[main] Główne okno utworzone.";

        glowneOkno.show();
        qInfo() << "[main] Wyświetlono główne okno.";

        int result = application.exec();
        qInfo() << "[main] Pętla aplikacji zakończona, kod wyjścia:" << result;

        closeLogger();
        return result;

    } catch (const std::exception &exception) {
        qCritical() << "[main] Wyjątek std::exception:" << exception.what();
    } catch (...) {
        qCritical() << "[main] Nieznany wyjątek podczas uruchamiania aplikacji.";
    }

    closeLogger();
    return -1;
}

/**
 * @brief Inicjalizuje logger aplikacji.
 *
 * Tworzy katalog logów w folderze AppData oraz plik logu z aktualnym timestampem.
 * Logger przechwytuje komunikaty `qDebug`, `qWarning`, `qInfo`, `qCritical`.
 */
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

/**
 * @brief Ustawia styl graficzny aplikacji.
 *
 * Zastosowany jest styl "Fusion" oraz modyfikowane są kolory dla elementów interfejsu (hover dla przycisków i list rozwijanych).
 *
 * @param[in,out] application Obiekt QApplication, którego wygląd będzie modyfikowany.
 */
void ustawStyl(QApplication &application)
{
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QPalette palette = application.palette();
    application.setStyleSheet("QPushButton:hover { background-color: #808080; }"
                              "QComboBox:hover { background-color: #808080; }");
    application.setPalette(palette);
}
