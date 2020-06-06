#ifndef LOGGER_H
#define LOGGER_H

#include "logging.h"

#include <QFile>
#include <QMessageLogContext>
#include <QTextStream>

class Logger
{
public:
    Logger();
    ~Logger();

    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    static QFile m_logFile;
    static QTextStream *m_logStream;
};

#endif // LOGGER_H
