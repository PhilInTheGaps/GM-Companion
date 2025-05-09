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

    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    static void enableSentryEvents(bool enable);

private:
    QFile m_logFile;
    inline static QTextStream m_logStream;
    inline static QMutex m_logMutex;
    inline static bool m_isSentryEnabled = false;

    static void createLogFileDir(const QString &filePath);
    void clearOldLog();

    static auto msgTypeToPrefix(QtMsgType type) -> QString;
};
