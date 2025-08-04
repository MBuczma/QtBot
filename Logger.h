/**
 * @file Logger.h
 * @brief Deklaracja funkcji loggera zapisującego logi do pliku.
 *
 * Logger przechwytuje komunikaty Qt (`qDebug`, `qInfo`, `qWarning`, `qCritical`) 
 * i zapisuje je do pliku tekstowego z timestampem. Może być używany w całej aplikacji.
 */
#ifndef LOGGER_H
#define LOGGER_H

#include <QMessageLogContext>
#include <QString>
#include <QtGlobal>

// Inicjuje logger (otwiera plik, instaluje customowy handler).
void initLogger(const QString &fileName);

// Customowy message handler: przechwytuje qDebug, qWarning, itp.
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

// Zamknięcie loggera (zwolnienie pliku, strumienia, przywrócenie starego handlera itp.)
void closeLogger();

#endif // LOGGER_H
