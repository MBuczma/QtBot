#include "Logger.h"
#include <QDebug>

// Użyjemy zmiennych statycznych, by otworzyć plik tylko raz na czas działania programu
static QFile logFile;
static QTextStream logStream;

void initLogger(const QString &fileName)
{
    // Otwieramy log w trybie append (dopisywanie na końcu pliku).
    // Jeśli chcesz nadpisywać plik od nowa, użyj QIODevice::WriteOnly
    // zamiast QIODevice::Append.
    logFile.setFileName(fileName);
    if (!logFile.open(QIODevice::Append | QIODevice::Text)) {
        // Jeśli otwarcie się nie uda, to wypisujemy ostrzeżenie.
        qWarning() << "Nie można otworzyć pliku logu:" << fileName;
        return;
    }
    logStream.setDevice(&logFile);

    // Instalujemy nasz message handler
    qInstallMessageHandler(customMessageHandler);
}

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Formatowanie wiadomości z uwzględnieniem czasu, typu komunikatu itp.
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

    // Dodatkowe informacje (np. plik i numer linii):
    // logMessage += QString("(%1:%2) ").arg(context.file).arg(context.line);

    // Wiadomość właściwa
    logMessage += msg;

    // Zapis do pliku
    logStream << logMessage << Qt::endl;
    logStream.flush();

    // Można także wyświetlić w konsoli (opcjonalnie):
    // fprintf(stderr, "%s\n", logMessage.toUtf8().constData());

    // Przy komunikacie typu Fatal wymuszone będzie zatrzymanie programu.
    if (type == QtFatalMsg) {
        abort();
    }
}

// Funkcja wywoływana np. w main() lub innej inicjalizacji
