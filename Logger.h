#ifndef LOGGER_H
#define LOGGER_H

#include <QMessageLogContext>
#include <QString>
#include <QtGlobal>

void initLogger(const QString &fileName);
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // LOGGER_H
