#include "logger.h"

#include <QDateTime>
#include <QDir>
#include <iostream>

QFile Logger::m_logFile;
QTextStream *Logger::m_logStream = nullptr;
QMutex *Logger::m_logMutex = nullptr;

Logger::Logger()
{
    QDir dir(QDir::homePath() + "/.gm-companion");
    if (!dir.exists())
    {
        QDir home(QDir::homePath());
        home.mkdir(".gm-companion");
    }

    m_logFile.setFileName(QDir::homePath() + "/.gm-companion/log.txt");

    // Clear old log
    if (m_logFile.open(QIODevice::WriteOnly))
    {
        m_logFile.write("");
        m_logFile.close();
    }

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
    QByteArray line = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ").toUtf8();

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

    *m_logStream << line << "\n";
    m_logStream->flush();
}
