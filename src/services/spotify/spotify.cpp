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

bool Spotify::instanceFlag = false;
Spotify *Spotify::single   = nullptr;

Spotify::Spotify(QObject *parent) : Service("Spotify", parent)
{
    m_networkManager = new QNetworkAccessManager(this);
    m_username = SettingsManager::getSetting("spotifyUsername", "", "Spotify");

    updateConnector();

    // Librespot signals
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit,                                &m_librespotProcess, &QProcess::kill);
    connect(&m_librespotProcess,          QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,                &Spotify::onLibrespotFinished);
    connect(&m_librespotProcess,          &QProcess::errorOccurred,                                      this,                &Spotify::onLibrespotError);
}

Spotify::~Spotify()
{
    instanceFlag = false;
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
    connect(m_connector, &RESTServiceConnector::receivedReply, this, &Spotify::onReceivedReply);
    connect(m_connector, &RESTServiceConnector::statusChanged, this, &Spotify::updateStatus);
    connect(m_connector, &RESTServiceConnector::isConnectedChanged, [ = ](const bool& connected) { setConnected(connected); });

    stopLibrespot();
    if (connected()) grant();
}

auto Spotify::getInstance()->Spotify *
{
    if (!instanceFlag)
    {
        single       = new Spotify;
        instanceFlag = true;
    }
    return single;
}

void Spotify::setDeviceActive()
{
    qCDebug(gmSpotify) << "Setting current machine as active device ...";

    // Get a list of all available devices.
    // Only the device name is not enough, we need the device id.

    QNetworkRequest request(QUrl("https://api.spotify.com/v1/me/player/devices"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);

    m_requestMap["devices"] = m_connector->get(request);
}

void Spotify::onReceivedDevices(const QByteArray& data)
{
    const auto devices = QJsonDocument::fromJson(data).object().value("devices").toArray();

    qCDebug(gmSpotify) << "Devices:" << devices;

    for (auto device : devices)
    {
        if ((device.toObject().value("name") == LIBRESPOT_DEVICE_NAME) &&
            !device.toObject().value("is_active").toBool())
        {
            // If current device was found and it is not active, make it active.
            QJsonObject parameters
            {
                { "device_ids", QJsonArray { device.toObject().value("id") } },
                { "play", false }
            };

            put(QUrl("https://api.spotify.com/v1/me/player"),
                QJsonDocument(parameters).toJson(QJsonDocument::JsonFormat::Compact));

            return;
        }
    }

    qCWarning(gmSpotify) << "Could not find spotify device" << LIBRESPOT_DEVICE_NAME;
}

void Spotify::connectService()
{
    setUsername(SettingsManager::getSetting("spotifyUsername", "", "Spotify"));
    updateConnector();
    grant();
}

void Spotify::disconnectService()
{
    setConnected(false);
    m_connector->disconnectService();
    SettingsManager::setPassword(m_username, "", "Spotify");
    SettingsManager::setSetting("spotifyUsername", "", "Spotify");
    SettingsManager::setSetting("spotifyID", "", "Spotify");
    SettingsManager::setSetting("spotifySecret", "", "Spotify");
    stopLibrespot();
}

auto Spotify::startLibrespot()->bool
{
    qCDebug(gmSpotify()) << "Starting librespot ...";
    updateStatus(StatusType::Info, tr("Starting librespot..."));

    if (!ProcessInfo::isProcessRunning(getLibrespotBinaryName().toStdString()))
    {
        auto username      = SettingsManager::getSetting("spotifyUsername", "", "Spotify");
        auto password      = SettingsManager::getPassword(username, "Spotify");
        auto librespotPath = getLibrespotPath();

        if (username.isEmpty())
        {
            qCWarning(gmSpotify()) << "Could not start librespot, username is not set.";
            updateStatus(StatusType::Error, tr("Error: Username is not set."));
            return false;
        }

        if (password.isEmpty())
        {
            qCWarning(gmSpotify()) << "Could not start librespot, password is not set.";
            updateStatus(StatusType::Error, tr("Error: Password is not set."));
            return false;
        }

        m_librespotProcess.start(librespotPath, { "-n", "GM-Companion",
                                                  "-u", username,
                                                  "-p", password
                                 });
    }
    else
    {
        qCWarning(gmSpotify()) << "Librespot is already running.";
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
        emit setConnected(false);
        updateStatus(StatusType::Error, tr("Error: Bad Credentials!"));
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

auto Spotify::getLibrespotPath()->QString
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

QString Spotify::getLibrespotBinaryName()
{
    #ifdef Q_OS_WIN
    return "librespot.exe";
    #endif

    return "librespot";
}

void Spotify::onReceivedReply(int id, QNetworkReply::NetworkError error, const QByteArray& data)
{
    if (error != QNetworkReply::NoError) {
        handleNetworkError(id, error, data);
    }

    if (m_requestMap["devices"] == id) {
        onReceivedDevices(data);
        return;
    }

    emit receivedReply(id, error, data);
}

void Spotify::handleNetworkError(int id, QNetworkReply::NetworkError error, const QByteArray& data)
{
    switch (error)
    {
    case QNetworkReply::ContentNotFoundError:
        qCInfo(gmSpotify) << "No active spotify device set. Will try to set the current machine.";
        setDeviceActive();
        break;

    case QNetworkReply::ContentAccessDenied:
        handleAccessDenied(data);
        break;

    default:
        qCWarning(gmSpotify) << "A network error occurred:" << id << error << data;
    }
}

void Spotify::handleAccessDenied(const QByteArray &data)
{
    auto error = QJsonDocument::fromJson(data).object()["error"].toObject();
    auto message = error["message"].toString();
    auto reason = error["reason"].toString();

    qCWarning(gmSpotify) << "Content Access Denied!" << message << reason;

    if (reason == "PREMIUM_REQUIRED")
    {
        setConnected(false);
        updateStatus(StatusType::Error, tr("Error: Spotify premium is required!"));
    }
    else
    {
        updateStatus(StatusType::Error, message);
    }
}

void Spotify::onAccessGranted()
{
    qCDebug(gmSpotify) << "Access has been granted!";
    setConnected(startLibrespot());
}
