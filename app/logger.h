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
    Logger(Logger &other) = delete;
    Logger(Logger &&other) = delete;
    Logger& operator =(const Logger &other) = delete;
    Logger& operator =(const Logger &&other) = delete;

    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    static constexpr const char* RELATIVE_LOGFILE_PATH = ".gm-companion/log.txt";

    inline static QFile m_logFile;
    inline static QTextStream m_logStream;
    inline static QMutex m_logMutex;

    static void createLogFileDir(const QString &filePath) ;
    static void clearOldLog();
};

#endif // LOGGER_H
