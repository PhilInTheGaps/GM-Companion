#pragma once

#include <QFile>
#include <QMessageLogContext>
#include <QMutex>
#include <QTextStream>

class Logger
{
public:
    Logger();

    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    QFile m_logFile;
    inline static QTextStream m_logStream;
    inline static QMutex m_logMutex;

    static void createLogFileDir(const QString &filePath);
    void clearOldLog();

    static auto msgTypeToPrefix(QtMsgType type) -> QString;
};
