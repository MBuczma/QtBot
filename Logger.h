/* Logger.h */
/*
 * Plik nagłówkowy Loggera zapisującego logi do pliku
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
