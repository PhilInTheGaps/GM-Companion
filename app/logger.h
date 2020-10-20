#ifndef LOGGER_H
#define LOGGER_H

#include "logging.h"

#include <QFile>
#include <QMessageLogContext>
#include <QTextStream>
#include <QMutexLocker>

class Logger
{
public:
    Logger();
    ~Logger();

    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    static QFile m_logFile;
    static QTextStream *m_logStream;
    static QMutex *m_logMutex;
};

#endif // LOGGER_H
