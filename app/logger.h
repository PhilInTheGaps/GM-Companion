#pragma once

#include <QFile>
#include <QMessageLogContext>
#include <QMutex>
#include <QTextStream>

class Logger
{
public:
    Logger();
    ~Logger();
    Q_DISABLE_COPY_MOVE(Logger);

    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    inline static QFile m_logFile;
    inline static QTextStream m_logStream;
    inline static QMutex m_logMutex;

    static void createLogFileDir(const QString &filePath);
    static void clearOldLog();

    static auto msgTypeToPrefix(QtMsgType type) -> QString;
};
