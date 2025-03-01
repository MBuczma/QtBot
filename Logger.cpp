/* Logger.c */
/*
 * Plik Loggera zapisującego logi do pliku
 */
#include "Logger.h"

#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <memory>

//Przechowujemy oryginalny handler, aby wywoływać go po naszym logowaniu.
static QtMessageHandler oldHandler = nullptr;
static std::unique_ptr<QFile> logFile;
static std::unique_ptr<QTextStream> logStream;
static bool loggerInitialized = false;

void initLogger(const QString &fileName)
{
    if (loggerInitialized) {
        // Sprawdzamy czy logger był już zainicjalizowany
        qWarning() << "Logger jest już zainicjalizowany!";
        return;
    }

    // Zapamiętujemy oryginalny handler, aby móc wywoływać go dalej
    oldHandler = qInstallMessageHandler(nullptr);

    // Tworzymy dynamicznie obiekt QFile
    logFile = std::make_unique<QFile>(fileName);
    if (!logFile->open(QIODevice::Append | QIODevice::Text)) {
        // Nie udało się otworzyć
        qWarning() << "Nie można otworzyć pliku logu:" << fileName;

        // Cofamy się do poprzedniego handlera
        qInstallMessageHandler(oldHandler);
        oldHandler = nullptr;

        logFile.reset();
        logStream.reset();
        return;
    }

    // Tworzymy strumień do zapisu w pliku
    logStream = std::make_unique<QTextStream>(logFile.get());

    // Instalujemy nasz handler
    qInstallMessageHandler(customMessageHandler);
    loggerInitialized = true;
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
        logStream->flush();
    }

    // Dodatkowo wywołujemy stary (oryginalny) handler,
    // aby komunikat pojawił się też w terminalu tak jak zwykle.
    if (oldHandler) {
        oldHandler(type, context, msg);
    }

    if (type == QtFatalMsg) {
        abort();
    }
}

void closeLogger()
{
    qDebug() << "Zamykam plik loggera";
    // Jeśli logger nie był w ogóle zainicjalizowany, nic nie robimy
    if (!loggerInitialized) {
        return;
    }

    // Przywracamy stary handler (żeby dalej mieć standardowe zachowanie, jeśli chcemy)
    qInstallMessageHandler(oldHandler);
    oldHandler = nullptr;
    loggerInitialized = false;

    // Zamykanie i reset wskaźników
    if (logStream) {
        logStream->flush(); // Dla pewności
        logStream.reset();  // Usuwa QTextStream
    }
    if (logFile) {
        if (logFile->isOpen()) {
            logFile->close();
        }
        logFile.reset(); // Usuwa QFile
    }
}
