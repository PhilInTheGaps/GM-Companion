#include "spotify.h"
#include "../services.h"
#include "logging.h"

#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>
#include <QNetworkRequest>
#include "o0globals.h"
#include "utils/processinfo.h"
#include "thirdparty/asyncfuture/asyncfuture.h"

Spotify::Spotify(QObject *parent) : Service("Spotify", parent)
{
    m_networkManager = new QNetworkAccessManager(this);
    username(SettingsManager::getSetting("spotifyUsername", "", "Spotify"));

    updateConnector();

    // Librespot signals
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit,                                &m_librespotProcess, &QProcess::kill);
    connect(&m_librespotProcess,          QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,                &Spotify::onLibrespotFinished);
    connect(&m_librespotProcess,          &QProcess::errorOccurred,                                      this,                &Spotify::onLibrespotError);
}

void Spotify::updateConnector()
{
    qCDebug(gmSpotify()) << "Updating spotify connector ...";

    delete m_connector;

    if (SettingsManager::getSetting("connection", "default", "Spotify") == "local")
    {
        m_connector = new SpotifyConnectorLocal(m_networkManager, new O2Spotify, this);
    }
    else
    {
        m_connector = new SpotifyConnectorServer(m_networkManager, this);
    }

    connect(m_connector, &RESTServiceConnector::accessGranted, this, &Spotify::onAccessGranted);
    connect(m_connector, &RESTServiceConnector::statusChanged, this, &Spotify::updateStatus);
    connect(m_connector, &RESTServiceConnector::isConnectedChanged, [ = ](const bool& connected) { this->connected(connected); });

    stopLibrespot();
    if (connected()) grant();
}

auto Spotify::getInstance() -> Spotify *
{
    if (!single)
    {
        single = new Spotify(nullptr);
    }
    return single;
}

auto Spotify::get(const QNetworkRequest &request) -> QFuture<RestNetworkReply *>
{
    if (connected())
    {
        return AsyncFuture::observe(m_connector->get(request))
                .subscribe([this](RestNetworkReply *reply)
        {
            handleNetworkError(reply);
            return reply;
        }).future();
    }

    return {};
}

auto Spotify::get(const QUrl &url) -> QFuture<RestNetworkReply *>
{
    if (connected())
    {
        return AsyncFuture::observe(m_connector->get(QNetworkRequest(url)))
                .subscribe([this](RestNetworkReply *reply)
        {
            handleNetworkError(reply);
            return reply;
        }).future();
    }

    return {};
}

auto Spotify::put(const QUrl &url, const QByteArray &data) -> QFuture<RestNetworkReply *>
{
    if (connected())
    {
        return AsyncFuture::observe(m_connector->put(QNetworkRequest(url), data))
                .subscribe([this](RestNetworkReply *reply)
        {
            handleNetworkError(reply);
            return reply;
        }).future();
    }

    return {};
}

auto Spotify::post(const QNetworkRequest& request, const QByteArray &data) -> QFuture<RestNetworkReply *>
{
    if (connected())
    {
        return AsyncFuture::observe(m_connector->post(request, data))
                .subscribe([this](RestNetworkReply *reply)
        {
            handleNetworkError(reply);
            return reply;
        }).future();
    }

    return {};
}


void Spotify::setDeviceActive()
{
    qCDebug(gmSpotify) << "Setting current machine as active device ...";

    // Get a list of all available devices.
    // Only the device name is not enough, we need the device id.

    QNetworkRequest request(QUrl("https://api.spotify.com/v1/me/player/devices"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, static_cast<const char*>(O2_MIME_TYPE_JSON));

    AsyncFuture::observe(m_connector->get(request))
            .subscribe([this](RestNetworkReply *reply) mutable { onReceivedDevices(reply); });
}

void Spotify::onReceivedDevices(const RestNetworkReply *reply)
{
    const auto data = reply->data();
    const auto devices = QJsonDocument::fromJson(data).object().value("devices").toArray();

    qCDebug(gmSpotify) << "Devices:" << devices;

    for (auto device : devices)
    {
        if (device.toObject().value("name") == LIBRESPOT_DEVICE_NAME)
        {
            if (!device.toObject().value("is_active").toBool())
            {
                // If current device was found and it is not active, make it active.
                QJsonObject parameters
                {
                    { "device_ids", QJsonArray { device.toObject().value("id") } },
                    { "play", false }
                };

                qCDebug(gmSpotify()) << "Found librespot instance \"" << LIBRESPOT_DEVICE_NAME <<
                                        "\" but it is inactive, setting as active device ...";

                auto future = put(QUrl("https://api.spotify.com/v1/me/player"),
                                  QJsonDocument(parameters).toJson(QJsonDocument::JsonFormat::Compact));

                AsyncFuture::observe(future).subscribe([this](RestNetworkReply *reply) {
                    if (reply->hasError())
                    {
                        updateStatus(ServiceStatus::Error, reply->errorText());
                    }
                    else
                    {
                        updateStatus(ServiceStatus::Success, tr("Connected."));
                    }
                    reply->deleteLater();
                }, [this]() {
                    updateStatus(ServiceStatus::Error, tr("Error: Could not start librespot."));
                });
            }
            else
            {
                qCDebug(gmSpotify()) << "Found librespot instance \"" << LIBRESPOT_DEVICE_NAME << "\", it is already set as the active device.";
            }

            return;
        }
    }

    qCWarning(gmSpotify) << "Could not find spotify device \"" << LIBRESPOT_DEVICE_NAME << "\". Trying again in a few seconds.";

    QTimer::singleShot(3000, this, &Spotify::setDeviceActive);
}

void Spotify::connectService()
{
    username(SettingsManager::getSetting("spotifyUsername", "", "Spotify"));
    updateConnector();
    grant();
}

void Spotify::disconnectService()
{
    connected(false);
    m_connector->disconnectService();
    SettingsManager::setPassword(username(), "", "Spotify");
    SettingsManager::setSetting("spotifyUsername", "", "Spotify");
    SettingsManager::setSetting("spotifyID", "", "Spotify");
    SettingsManager::setSetting("spotifySecret", "", "Spotify");
    stopLibrespot();
}

auto Spotify::startLibrespot() -> bool
{
    qCDebug(gmSpotify()) << "Starting librespot ...";
    updateStatus(ServiceStatus::Type::Info, tr("Starting librespot..."));

    if (!ProcessInfo::isProcessRunning(getLibrespotBinaryName()))
    {
        auto username      = SettingsManager::getSetting("spotifyUsername", "", "Spotify");
        auto password      = SettingsManager::getPassword(username, "Spotify");
        auto librespotPath = getLibrespotPath();

        if (username.isEmpty())
        {
            qCWarning(gmSpotify()) << "Could not start librespot, username is not set.";
            updateStatus(ServiceStatus::Type::Error, tr("Error: Username is not set."));
            return false;
        }

        if (password.isEmpty())
        {
            qCWarning(gmSpotify()) << "Could not start librespot, password is not set.";
            updateStatus(ServiceStatus::Type::Error, tr("Error: Password is not set."));
            return false;
        }

        connect(&m_librespotProcess, &QProcess::readyReadStandardError, this, [this]() {
            const auto output = m_librespotProcess.readAllStandardError();

            if (output.contains("WARN"))
            {
                qCWarning(gmSpotify()) << "LIBRESPOT:" << output;
                updateStatus(ServiceStatus::Warning, output);
            }

            if (output.contains("Connection failed:"))
            {
                updateStatus(ServiceStatus::Error, output);
                stopLibrespot();
            }
        });

        QStringList args;
        args << "-n" << "GM-Companion" << "-u" << username << "-p" << password;
        m_librespotProcess.start(librespotPath, args);
    }
    else
    {
        qCWarning(gmSpotify()) << "Librespot is already running.";
        updateStatus(ServiceStatus::Type::Warning, tr("Librespot process is already running."));
    }

    setDeviceActive();
    return true;
}

void Spotify::stopLibrespot()
{
    if (m_librespotProcess.state() != QProcess::NotRunning)
    {
        qCDebug(gmSpotify()) << "Stopping librespot ...";
        m_librespotProcess.terminate();
        m_librespotProcess.waitForFinished(1000);
    }
}

void Spotify::onLibrespotFinished(const int& exitCode, const QProcess::ExitStatus& exitStatus)
{
    qCWarning(gmSpotify()) << "Librespot exited with code" << exitCode << exitStatus;

    switch (exitCode)
    {
    case 101: // BadCredentials
        qCWarning(gmSpotify()) << "Spotify credentials are wrong!";
        connected(false);
        updateStatus(ServiceStatus::Type::Error, tr("Error: Bad Credentials!"));
        break;

    default:
        break;
    }
}

void Spotify::onLibrespotError(const QProcess::ProcessError& error)
{
    qCWarning(gmSpotify) << "An error occurred with librespot:" << error;
    qCWarning(gmSpotify) << m_librespotProcess.errorString();
}

auto Spotify::getLibrespotPath() -> QString
{
    auto binaryName = getLibrespotBinaryName();

    // First check next to gm-companion binary
    auto librespotPath = QCoreApplication::applicationDirPath() + "/" + binaryName;

    if (QFile(librespotPath).exists()) {
        qCDebug(gmSpotify) << "Using local librespot binary at" << librespotPath << ".";
        return librespotPath;
    }

    // Then check in cargo build directory
    librespotPath = QDir::homePath() + "/.cargo/bin/" + binaryName;

    if (QFile(librespotPath).exists()) {
        qCDebug(gmSpotify) << "Using librespot binary in cargo path:" << librespotPath << ".";
        return librespotPath;
    }

    // Finally try to execute from PATH
    qCDebug(gmSpotify) << "Trying to use system version of librespot ...";
    return binaryName;
}

constexpr auto Spotify::getLibrespotBinaryName() -> const char*
{
    #ifdef Q_OS_WIN
    return "librespot.exe";
    #else
    return "librespot";
    #endif
}

void Spotify::handleNetworkError(const RestNetworkReply *reply)
{
    if (!reply)
    {
        qCCritical(gmSpotify()) << "Error: trying to handle network error, but reply is null!";
        return;
    }

    switch (reply->error())
    {
    case QNetworkReply::NoError:
        return;

    case QNetworkReply::ContentNotFoundError:
        qCInfo(gmSpotify) << "No active spotify device set. Will try to set the current machine.";
        setDeviceActive();
        break;

    case QNetworkReply::ContentAccessDenied:
        handleAccessDenied(SpotifyNetworkError(reply->data()));
        break;

    default:
        qCWarning(gmSpotify) << "A network error occurred:" << reply->error() << reply->data();
    }
}

void Spotify::handleAccessDenied(const SpotifyNetworkError &error)
{
    qCWarning(gmSpotify) << "Content Access Denied!" << error;

    if (error.reason() == "PREMIUM_REQUIRED")
    {
        connected(false);
        updateStatus(ServiceStatus::Type::Error, tr("Error: Spotify premium is required!"));
    }
    else
    {
        updateStatus(ServiceStatus::Type::Error, error.message());
    }
}

void Spotify::onAccessGranted()
{
    qCDebug(gmSpotify) << "Access has been granted!";
    connected(startLibrespot());
}
