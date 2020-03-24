#include "spotify.h"
#include "services.h"
#include "logging.h"

#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QByteArray>
#include <QUrlQuery>
#include <QHostInfo>
#include <QCoreApplication>
#include <QNetworkRequest>
#include "o0globals.h"
#include "utils/processinfo.h"

bool Spotify::instanceFlag = false;
Spotify *Spotify::single   = nullptr;

Spotify::Spotify()
{
    m_networkManager = new QNetworkAccessManager;

    updateConnector();

    // Librespot signals
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, &m_librespotProcess, &QProcess::kill);
    connect(&m_librespotProcess,          QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [ = ](int exitCode, QProcess::ExitStatus exitStatus) {
        emit wrongPassword();
        qCCritical(gmSpotify()) << "Librespot exited with code" << exitCode << exitStatus;
    });
    connect(&m_librespotProcess, &QProcess::errorOccurred, [ = ](QProcess::ProcessError error) {
        qCCritical(gmSpotify) << "An error occurred with librespot:" << error;
        qCCritical(gmSpotify) << m_librespotProcess.errorString();
    });
}

Spotify::~Spotify()
{
    instanceFlag = false;
    m_connector->deleteLater();
    m_networkManager->deleteLater();
}

void Spotify::updateConnector()
{
    qCDebug(gmSpotify()) << "Updating spotify connector ...";

    if (m_connector)
    {
        delete m_connector;
    }

    if (m_sManager.getSetting(Setting::serviceConnection) == "local")
    {
        m_connector = new SpotifyConnectorLocal(m_networkManager);
    }
    else
    {
        m_connector = new SpotifyConnectorServer(m_networkManager);
    }

    // Connector signals
    connect(m_connector, &ISpotifyConnector::accessGranted, this, &Spotify::onAccessGranted);
    connect(m_connector, &ISpotifyConnector::receivedReply, this, &Spotify::onReceivedReply);

    m_connector->grantAccess();
}

/**
 * @brief Get the type of a spotify uri.
 * @param uri Spotify URI like spotify:track:6rqhFgbbKwnb9MLmUQDhG6
 * @return 0: Album, 1: Playlist, 2: Track, 3: Artist, 4: episode, 5: show -1:
 * unknown
 */
int Spotify::getUriType(QString uri)
{
    if (uri.contains("album:")) return 0;

    if (uri.contains("playlist:")) return 1;

    if (uri.contains("track:")) return 2;

    if (uri.contains("artist:")) return 3;

    if (uri.contains("episode:")) return 4;

    if (uri.contains("show:")) return 5;

    return -1;
}

/**
 * @brief Get Spotify ID from Spotify URI
 * @param uri Spotify URI like spotify:track:6rqhFgbbKwnb9MLmUQDhG6
 * @return Spotify ID like 6rqhFgbbKwnb9MLmUQDhG6
 */
QString Spotify::getId(QString uri)
{
    return uri = uri.right(uri.count() - uri.lastIndexOf(":") - 1);
}

Spotify * Spotify::getInstance()
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

void Spotify::onReceivedDevices(QByteArray data)
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

    qCCritical(gmSpotify) << "Could not find spotify device" << LIBRESPOT_DEVICE_NAME;
}

void Spotify::startLibrespot()
{
    ProcessInfo pi;

    if (pi.getProcIdByName("librespot") == -1)
    {
        auto username      = m_sManager.getSetting(Setting::spotifyUsername);
        auto password      = m_sManager.getSetting(Setting::spotifyPassword);
        auto librespotPath = getLibrespotPath();

        if (username.isEmpty() || password.isEmpty()) return;

        m_librespotProcess.start(librespotPath, { "-n", "GM-Companion",
                                                  "-u", username,
                                                  "-p", password
                                 });
    }

    setDeviceActive();
}

QString Spotify::getLibrespotPath()
{
    auto binaryName = "librespot";

    #ifdef Q_OS_WIN
    binaryName = "librespot.exe"
    #endif // ifdef Q_OS_WIN

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

void Spotify::onReceivedReply(int id, QNetworkReply::NetworkError error, QByteArray data)
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

void Spotify::handleNetworkError(int id, QNetworkReply::NetworkError error, QByteArray data)
{
    switch (error)
    {
    case QNetworkReply::ContentNotFoundError:
        qCWarning(gmSpotify) << "No active spotify device set. Will try to set the current machine.";
        setDeviceActive();
        break;

    default:
        qCCritical(gmSpotify) << "A network error occurred:" << id << error << data;
    }
}

void Spotify::onAccessGranted()
{
    qCDebug(gmSpotify) << "Access has been granted!";
    startLibrespot();
}
