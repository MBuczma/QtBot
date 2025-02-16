#ifndef LOGGER_H
#define LOGGER_H

#include <QDateTime>
#include <QFile>
#include <QMessageLogContext>
#include <QString>
#include <QTextStream>

void initLogger(const QString &fileName);

// Deklaracja funkcji
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // LOGGER_H
