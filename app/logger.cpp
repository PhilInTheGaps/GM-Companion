#include "logger.h"
#include "common/utils/fileutils.h"
#include "messages/messagemanager.h"
#include <QDateTime>
#include <QDir>
#include <QLoggingCategory>
#include <QMutexLocker>
#include <iostream>
#include <sentry.h>

Q_LOGGING_CATEGORY(gmLogger, "gm.logger")

static constexpr auto RELATIVE_LOGFILE_PATH = ".gm-companion/log.txt";

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
    m_logStream.flush();
    m_logStream.setDevice(nullptr);
}

static auto msgTypeToSentryLevel(QtMsgType type) -> sentry_level_t
{
    switch (type)
    {
    case QtInfoMsg:
        return SENTRY_LEVEL_INFO;
    case QtWarningMsg:
        return SENTRY_LEVEL_WARNING;
    case QtCriticalMsg:
        return SENTRY_LEVEL_ERROR;
    case QtFatalMsg:
        return SENTRY_LEVEL_FATAL;
    default:
        return SENTRY_LEVEL_DEBUG;
    }
}

void Logger::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    auto timestamp = QDateTime::currentDateTime();
    auto line = QStringLiteral("%1 %2 %3: %4")
                    .arg(timestamp.toString(QStringLiteral("yyyy-MM-dd hh:mm:ss.zzz ")).toUtf8(), msgTypeToPrefix(type),
                         context.category, msg.toUtf8())
                    .toUtf8()
                    .constData();

    QMutexLocker const locker(&m_logMutex);

    if (type == QtInfoMsg || type == QtDebugMsg)
    {
        std::cout << line << '\n' << std::flush;
    }
    else
    {
        std::cerr << line << '\n' << std::flush;
    }

    if (type != QtDebugMsg)
    {
        MessageManager::instance()->addMessage(timestamp, type, context.category, msg);

        if (m_isSentryEnabled)
        {
            auto sentryContext = sentry_value_new_object();
            sentry_value_set_by_key(sentryContext, "category", sentry_value_new_string(context.category));
            sentry_value_set_by_key(sentryContext, "file", sentry_value_new_string(context.file));
            sentry_value_set_by_key(sentryContext, "function", sentry_value_new_string(context.function));
            sentry_value_set_by_key(sentryContext, "line", sentry_value_new_int32(context.line));
            sentry_value_set_by_key(sentryContext, "version", sentry_value_new_int32(context.version));
            sentry_set_context("QMessageLogContext", sentryContext);

            sentry_capture_event(sentry_value_new_message_event(msgTypeToSentryLevel(type), nullptr, msg.toUtf8()));
        }
    }

    m_logStream << line << '\n';
}

void Logger::enableSentryEvents(bool enable)
{
    m_isSentryEnabled = enable;
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

auto Logger::msgTypeToPrefix(QtMsgType type) -> QString
{
    switch (type)
    {
    case QtInfoMsg:
        return QStringLiteral("INF");
    case QtDebugMsg:
        return QStringLiteral("DBG");
    case QtWarningMsg:
        return QStringLiteral("WRN");
    case QtCriticalMsg:
        return QStringLiteral("CRT");
    case QtFatalMsg:
        return QStringLiteral("FTL");
    default:
        qCritical() << "Error: Unexpected log message type" << type;
        return QStringLiteral("UKN");
    }
}
