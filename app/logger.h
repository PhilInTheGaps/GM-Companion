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
    static constexpr const char* RELATIVE_LOGFILE_PATH = ".gm-companion/log.txt";

    inline static QFile m_logFile;
    inline static QTextStream *m_logStream = nullptr;
    inline static QMutex *m_logMutex = nullptr;

    void createLogFileDir(const QString &filePath);
    void clearOldLog();
};

#endif // LOGGER_H
