#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QMessageLogContext>
#include <QMutexLocker>
#include <QTextStream>

class Logger
{
public:
    Logger();
    ~Logger();
    Q_DISABLE_COPY_MOVE(Logger);

    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    static constexpr const char *RELATIVE_LOGFILE_PATH = ".gm-companion/log.txt";

    inline static QFile m_logFile;
    inline static QTextStream m_logStream;
    inline static QMutex m_logMutex;

    static void createLogFileDir(const QString &filePath);
    static void clearOldLog();

    static auto msgTypeToPrefix(QtMsgType type) -> QString;
};

#endif // LOGGER_H
