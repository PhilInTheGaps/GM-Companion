#include "logger.h"
#include "messages/messagemanager.h"
#include "messages/messagedispatcher.h"
#include "common/utils/fileutils.h"

#include <QDateTime>
#include <QDir>
#include <iostream>

Logger::Logger()
{
    const auto fullFilePath = FileUtils::fileInDir(RELATIVE_LOGFILE_PATH, QDir::homePath());
    m_logFile.setFileName(fullFilePath);

    createLogFileDir(fullFilePath);
    clearOldLog();

    // Open for writing
    if (!m_logFile.open(QIODevice::Append | QIODevice::Text))
    {
        qCWarning(gmMain()) << "Error: Could not open log file at" << m_logFile.fileName();
    }

    m_logStream = new QTextStream(&m_logFile);
    m_logMutex = new QMutex();

    qInstallMessageHandler(messageHandler);
}

Logger::~Logger()
{
    if (m_logFile.isOpen())
    {
        m_logFile.close();
    }

    delete m_logStream;
    delete m_logMutex;
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
    }

    // Write to the output category of the message and the message itself
    line.append(context.category).append(": ").append(msg.toUtf8());

    QMutexLocker locker(m_logMutex);

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
        auto *dispatcher = new MessageDispatcher(timestamp, type, msg);
//        MessageManager::instance()->addMessage(timestamp, type, msg);
    }

    *m_logStream << line << "\n";
    m_logStream->flush();
}

/// If it does not exist yet, create the folder that will contain the log file
void Logger::createLogFileDir(const QString &filePath)
{
    const auto fileDir = FileUtils::dirFromPath(filePath);
    QDir dir(fileDir);

    if (!dir.exists())
    {
        if (!dir.mkpath(dir.path()))
        {
            qCWarning(gmMain()) << "Error: Create dir for log file:" << fileDir;
        }
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
