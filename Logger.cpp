/**
 * @file Logger.cpp
 * @brief Implementacja loggera zapisującego logi do pliku.
 *
 * Plik zawiera implementację funkcji odpowiedzialnych za:
 * - Inicjalizację loggera i otwarcie pliku
 * - Przechwytywanie wiadomości Qt i zapis do pliku
 * - Zamykanie loggera i przywracanie domyślnego handlera
 */
#include "Logger.h"

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QTextStream>

/// Oryginalny handler Qt przechowywany w celu wywoływania go po logowaniu własnym
static QtMessageHandler oldHandler = nullptr;
/// Wskaźnik na otwarty plik logu
static std::unique_ptr<QFile> logFile;
/// Strumień do zapisu danych do pliku
static std::unique_ptr<QTextStream> logStream;

void initLogger(const QString &fileName)
{
    // Zapamiętujemy oryginalny handler, aby móc wywoływać go dalej
    oldHandler = qInstallMessageHandler(nullptr);

    // Tworzymy dynamicznie obiekt QFile
    logFile = std::make_unique<QFile>(fileName);
    if (!logFile->open(QIODevice::Append | QIODevice::Text)) {
        // Nie udało się otworzyć
        qWarning() << "[Logger][initLogger]Nie można otworzyć pliku logu:" << fileName;

        // Cofamy się do poprzedniego handlera
        qInstallMessageHandler(oldHandler);
        oldHandler = nullptr;

        logFile.reset();
        logStream.reset();
        return;
    }

    // Tworzymy strumień do zapisu w pliku
    logStream = std::make_unique<QTextStream>(logFile.get());

    // Instalujemy handler
    qInstallMessageHandler(customMessageHandler);
}

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Zapis do pliku (o ile został poprawnie otwarty w initLogger)
    if (logStream) {
        QString logMessage = QString("[%1] ").arg(
            QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));

        switch (type) {
        case QtDebugMsg:
            logMessage += "[Debug] ";
            break;
        case QtInfoMsg:
            logMessage += "[Info] ";
            break;
        case QtWarningMsg:
            logMessage += "[Warning] ";
            break;
        case QtCriticalMsg:
            logMessage += "[Critical] ";
            break;
        case QtFatalMsg:
            logMessage += "[Fatal] ";
            break;
        }

        logMessage += msg;

        (*logStream) << logMessage << Qt::endl;
    }

    // Dodatkowo wywołujemy stary (oryginalny) handler aby komunikat pojawił się też w terminalu tak jak zwykle.
    if (oldHandler) {
        oldHandler(type, context, msg);
    }

    if (type == QtFatalMsg) {
        if (logStream) {
            logStream->flush();
        }
        abort();
    }
}

void closeLogger()
{
    qDebug() << "[Logger][closeLogger] Zamykam plik loggera";

    // Przywracamy stary handler (żeby dalej mieć standardowe zachowanie)
    qInstallMessageHandler(oldHandler);
    oldHandler = nullptr;

    // Zamykanie i reset wskaźników
    if (logStream) {
        logStream.reset();  // Usuwa QTextStream
    }
    if (logFile && logFile->isOpen()) {
        logFile->close();
    }
    logFile.reset();
}
