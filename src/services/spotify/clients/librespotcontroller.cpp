#include "librespotcontroller.h"
#include "settings/settingsmanager.h"
#include "thirdparty/asyncfuture/asyncfuture.h"
#include "updates/updatemanager.h"
#include "utils/fileutils.h"
#include "utils/processinfo.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QLoggingCategory>
#include <QNetworkRequest>
#include <QTimer>
#include <chrono>

Q_LOGGING_CATEGORY(gmLibrespotController, "gm.service.spotify.clients.librespot")

using namespace AsyncFuture;

LibrespotController::LibrespotController(QObject *parent)
    : AbstractSpotifyClientController(parent, gmLibrespotController())
{
    initProcess();
}

auto LibrespotController::start() -> QFuture<bool>
{
    updateStatus(ServiceStatus::Info, "Starting librespot client ...");

    if (isOtherProcessIsRunning())
    {
        generateDeviceName(true);
        qCWarning(gmLibrespotController())
            << "At second librespot process is already running, this might lead to unexpected behavior.";
    }

    m_isExitExpected = false;

    const auto username = SettingsManager::instance()->get<QString>(QStringLiteral("spotifyUsername"), QLatin1String(),
                                                                    QStringLiteral("Spotify"));
    const auto password = SettingsManager::getPassword(username, QStringLiteral("Spotify"));

    if (username.isEmpty())
    {
        qCWarning(gmLibrespotController()) << "Could not start librespot, username is not set.";
        updateStatus(ServiceStatus::Type::Error, tr("Error: Username is not set."));
        return completed(false);
    }

    const auto callback = [this, username](const QString &password) {
        if (password.isEmpty())
        {
            qCWarning(gmLibrespotController()) << "Could not start librespot, password is not set.";
            updateStatus(ServiceStatus::Type::Error, tr("Error: Password is not set."));
            return completed(false);
        }

        const auto info = getLibrespotInfo();
        if (info.version.isEmpty())
        {
            qCWarning(gmLibrespotController()) << "Could not get librespot version number.";
            updateStatus(ServiceStatus::Type::Error,
                         tr("Error: Could not get librespot version number. "
                            "This probably means that the librespot executable can not be found."));
            return completed(false);
        }

        qCDebug(gmLibrespotController()) << QString(info);
        m_hasAuthenticated = deferred<bool>();

        const auto args = getLibrespotArgs(username);
        const auto librespotPath = getLibrespotPath();

        qCDebug(gmLibrespotController()) << librespotPath << args;

        if (m_librespotProcess.state() == QProcess::Running || QProcess::Starting)
        {
            stop();
        }

        // In librespot versions > 0.3.1 passing the password via stdin is broken
        // but older versions don't support passing the pw via env variables
        if (UpdateManager::compareVersions(info.version, QStringLiteral("0.3.1")))
        {
            auto env = QProcessEnvironment::systemEnvironment();
            env.insert(QStringLiteral("LIBRESPOT_PASSWORD"), password);
            m_librespotProcess.setProcessEnvironment(env);

            m_librespotProcess.start(librespotPath, args);
        }
        else
        {
            m_librespotProcess.start(librespotPath, args);
            m_librespotProcess.write(QStringLiteral("%1\n").arg(password).toUtf8()); // pass password via stdin
        }

        return m_hasAuthenticated.future();
    };

    return observe(password).subscribe(callback).future();
}

auto LibrespotController::stop() -> bool
{
    using namespace std::chrono_literals;

    if (m_librespotProcess.state() == QProcess::NotRunning) return true;

    updateStatus(ServiceStatus::Info, tr("Stopping librespot client ..."));

    m_isExitExpected = true;
    m_librespotProcess.terminate();

    if (!m_librespotProcess.waitForFinished(PROCESS_TERMINATE_TIMEOUT_MS))
    {
        updateStatus(ServiceStatus::Error, tr("Librespot thread could not be terminated, trying to kill it now ..."));
        m_librespotProcess.kill();

        if (!m_librespotProcess.waitForFinished(PROCESS_TERMINATE_TIMEOUT_MS))
        {
            updateStatus(ServiceStatus::Error, tr("Librespot thread could not be killed."));
            return false;
        }
    }

    updateStatus(ServiceStatus::Info, tr("Librespot client has stopped."));
    return true;
}

auto LibrespotController::hasStarted() const -> bool
{
    return m_hasStarted;
}

void LibrespotController::initProcess()
{
#ifdef Q_OS_WINDOWS
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, &m_librespotProcess, &QProcess::kill);
#else
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, &m_librespotProcess, &QProcess::terminate);
#endif

    connect(&m_librespotProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            &LibrespotController::onLibrespotFinished);
    connect(&m_librespotProcess, &QProcess::errorOccurred, this, &LibrespotController::onLibrespotError);
    connect(&m_librespotProcess, &QProcess::readyReadStandardError, this, &LibrespotController::onLibrespotOutputReady);
}

void LibrespotController::setAsActiveDevice()
{
    qCDebug(gmLibrespotController()) << "Setting librespot instance as active device ...";

    const auto callback = [this](const QSharedPointer<SpotifyDevice> &device) {
        if (device.isNull() || device->id.isEmpty())
        {
            if (m_tryAgainIfSettingActiveFails)
            {
                m_tryAgainIfSettingActiveFails = false;
                qCDebug(gmLibrespotController())
                    << "Could not find spotify device" << deviceName() << "-> trying again in a few seconds.";
                QTimer::singleShot(TRY_AGAIN_TIMEOUT_MS, this, &LibrespotController::setAsActiveDevice);
            }
            else
            {
                qCCritical(gmLibrespotController()) << "Could not find spotify device" << deviceName();
                updateStatus(ServiceStatus::Error,
                             tr("Spotify device could not be found. Are the credentials correct?"));
            }
            return false;
        }

        if (device->isActive)
        {
            qCDebug(gmLibrespotController())
                << "Found librespot instance" << deviceName() << "-> it is already set as the active device.";
            return true;
        }

        setActiveDevice(*device);
        return true;
    };

    const auto future = getDevice(deviceName());
    observe(future).subscribe(callback);
}

auto LibrespotController::isOtherProcessIsRunning() -> bool
{
    return ProcessInfo::isProcessRunning(getLibrespotBinaryName());
}

auto LibrespotController::getLibrespotPath() -> QString
{
    const auto *const binaryName = getLibrespotBinaryName();

    // First check next to gm-companion binary
    auto librespotPath = FileUtils::fileInDir(binaryName, QCoreApplication::applicationDirPath());

    if (QFile(librespotPath).exists())
    {
        qCDebug(gmLibrespotController()) << "Using local librespot binary at" << librespotPath;
        return librespotPath;
    }

    // Then check in cargo build directory
    librespotPath = QDir::homePath() + "/.cargo/bin/" + binaryName;

    if (QFile(librespotPath).exists())
    {
        qCDebug(gmLibrespotController) << "Using librespot binary in cargo path:" << librespotPath;
        return librespotPath;
    }

    // Finally try to execute from PATH
    qCDebug(gmLibrespotController) << "Trying to use system version of librespot in PATH ...";
    return binaryName;
}

constexpr auto LibrespotController::getLibrespotBinaryName() -> const char *
{
#ifdef Q_OS_WIN
    return "librespot.exe";
#else
    return "librespot";
#endif
}

auto LibrespotController::getLibrespotArgs(const QString &username) const -> QStringList
{
    QStringList args;

    args << "-n" << deviceName();
    args << "-u" << username;
    args << "-b"
         << SettingsManager::instance()->get(QStringLiteral("bitrate"), QStringLiteral("160"),
                                             QStringLiteral("Spotify"));
    args << "--volume-ctrl"
         << "linear";
    args << "--volume-range"
         << "60.0";

    if (!SettingsManager::instance()->get(QStringLiteral("enableCache"), true, QStringLiteral("Spotify")))
    {
        args << "--disable-audio-cache";
    }

    if (SettingsManager::instance()->get(QStringLiteral("enableVolumeNormalization"), false, QStringLiteral("Spotify")))
    {
        args << "--enable-volume-normalisation";
    }

    return args;
}

auto LibrespotController::getLibrespotInfo() -> LibrespotInfo
{
    m_librespotProcess.start(getLibrespotPath(), {"--version"});
    m_librespotProcess.waitForFinished();
    const auto output = m_librespotProcess.readAll();

    const auto regex = QRegularExpression(R"((?'version'(\d\.?)+)\s+(?'commit'\w*).*(?'date'\d{4}-\d{2}-\d{2}))");
    const auto match = regex.match(output);

    return {match.captured("version"), match.captured("commit"),
            QDate::fromString(match.captured("date"), QStringLiteral("yyyy-MM-dd"))};
}

void LibrespotController::onLibrespotFinished(int exitCode, const QProcess::ExitStatus &exitStatus)
{
    m_hasStarted = false;

    if (exitCode == 0 || m_isExitExpected) return;

    qCWarning(gmLibrespotController()) << "Librespot exited with code" << exitCode << exitStatus;
}

void LibrespotController::onLibrespotError(const QProcess::ProcessError &error)
{
    if (m_isExitExpected) return;
    qCWarning(gmLibrespotController()) << "An error occurred with librespot:" << error;
    qCWarning(gmLibrespotController()) << m_librespotProcess.errorString();
}

void LibrespotController::onLibrespotOutputReady()
{
    const auto output = m_librespotProcess.readAllStandardError();

    for (const auto &line : output.split('\n'))
    {
        if (line.isEmpty()) continue;

        if (line.contains("WARN"))
        {
            // for some reason "ignoring blacklisted access point ..."
            // messages are categorized as warnings, so we want to handle them as debug info
            if (line.contains("Ignoring"))
            {
                qCDebug(gmLibrespotController()) << "LIBRESPOT:" << line;
                continue;
            }

            qCWarning(gmLibrespotController()) << "LIBRESPOT:" << line;
            updateStatus(ServiceStatus::Warning, line);
        }
        else if (line.contains("ERROR"))
        {
            qCWarning(gmLibrespotController()) << "LIBRESPOT:" << line;
            updateStatus(ServiceStatus::Error, line);
        }
        else
        {
            qCDebug(gmLibrespotController()) << "LIBRESPOT:" << line;
        }

        if (line.contains("Authenticated as"))
        {
            m_hasStarted = true;
            m_hasAuthenticated.complete(true);
            updateStatus(ServiceStatus::Success,
                         tr("Successfully started librespot client. (Spotify device %1)").arg(deviceName()));
        }

        if (line.contains("Connection failed:"))
        {
            m_hasAuthenticated.complete(false);
            updateStatus(ServiceStatus::Error, line);
            stop();
        }
    }
}
