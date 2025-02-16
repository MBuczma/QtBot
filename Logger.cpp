#include "Logger.h"
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QTextStream>

// Zmienna globalna (statyczna) przechowująca wskaźnik do starego handlera
static QtMessageHandler g_originalHandler = nullptr;

// Plik i strumień do logu
static QFile logFile;
static QTextStream logStream;

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // --- Zapis do pliku ---
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

    logStream << logMessage << Qt::endl;
    logStream.flush();

    // --- Wywołanie oryginalnego handlera (Qt) ---
    if (g_originalHandler) {
        // Warto przekazać oryginalny msg, a nie nasz logMessage,
        // bo oryginalny handler sam sobie sformatuje informacje
        // (np. z pliku, linii, itp.).
        g_originalHandler(type, context, msg);
    }

    if (type == QtFatalMsg) {
        abort();
    }
}

void initLogger(const QString &fileName)
{
    // Zapamiętujemy stary handler, żeby móc go wywoływać
    g_originalHandler = qInstallMessageHandler(nullptr);

    logFile.setFileName(fileName);
    if (!logFile.open(QIODevice::Append | QIODevice::Text)) {
        // Tutaj jeszcze g_originalHandler może być wywołany
        qWarning() << "Nie można otworzyć pliku logu:" << fileName;
    } else {
        logStream.setDevice(&logFile);
    }

    // Teraz instalujemy nasz handler
    qInstallMessageHandler(customMessageHandler);
}
