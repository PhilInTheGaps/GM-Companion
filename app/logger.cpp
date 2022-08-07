#include "logger.h"
#include "messages/messagemanager.h"
#include "messages/messagedispatcher.h"
#include "common/utils/fileutils.h"

#include <QDateTime>
#include <QDir>
#include <QLoggingCategory>
#include <iostream>

Q_LOGGING_CATEGORY(gmLogger, "gm.logger")

Logger::Logger()
{
    const auto fullFilePath = FileUtils::fileInDir(RELATIVE_LOGFILE_PATH, QDir::homePath());
    m_logFile.setFileName(fullFilePath);

    createLogFileDir(fullFilePath);
    clearOldLog();

    // Open for writing
    if (!m_logFile.open(QIODevice::Append | QIODevice::Text))
    {
        qCWarning(gmLogger()) << "Error: Could not open log file at" << m_logFile.fileName();
    }

    m_logStream.setDevice(&m_logFile);

    qInstallMessageHandler(messageHandler);
}

Logger::~Logger()
{
    if (m_logFile.isOpen())
    {
        m_logFile.close();
    }
}

void Logger::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    auto timestamp = QDateTime::currentDateTime();
    auto line = timestamp.toString("yyyy-MM-dd hh:mm:ss.zzz ").toUtf8();

    // By type determine to what level belongs message
    switch (type)
    {
    case QtInfoMsg:     line.append("INF "); break;
    case QtDebugMsg:    line.append("DBG "); break;
    case QtWarningMsg:  line.append("WRN "); break;
    case QtCriticalMsg: line.append("CRT "); break;
    case QtFatalMsg:    line.append("FTL "); break;
    default: qCritical() << "Error: Unexpected log message type" << type;
    }

    // Write to the output category of the message and the message itself
    line.append(context.category).append(": ").append(msg.toUtf8());

    QMutexLocker locker(&m_logMutex);

    if (type == QtInfoMsg || type == QtDebugMsg)
    {
        std::cout << line.toStdString() << std::endl;
    }
    else
    {
        std::cerr << line.toStdString() << std::endl;
    }

    if (type != QtDebugMsg)
    {
        MessageDispatcher::dispatch(timestamp, type, context.category, msg);
    }

    m_logStream << line << "\n";
    m_logStream.flush();
}

/// If it does not exist yet, create the folder that will contain the log file
void Logger::createLogFileDir(const QString &filePath)
{
    const auto fileDir = FileUtils::dirFromPath(filePath);
    const QDir dir(fileDir);

    if (!dir.exists() || !dir.mkpath(dir.path()))
    {
        qCWarning(gmLogger()) << "Error: Create dir for log file:" << fileDir;
    }
}

/// Remove everything from the log file
void Logger::clearOldLog()
{
    if (m_logFile.open(QIODevice::WriteOnly))
    {
        m_logFile.write("");
        m_logFile.close();
    }
}
