#include "spotify.h"
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

#include "utils/processinfo.h"

#include "o0settingsstore.h"
#include "o0requestparameter.h"
#include "o0globals.h"

bool Spotify::instanceFlag = false;
Spotify *Spotify::single   = nullptr;

Spotify::Spotify()
{
    m_o2spotify      = new O2Spotify;
    m_networkManager = new QNetworkAccessManager;

    // Settings Store for tokens
    // TODO: Replace with secure keychain or something similar
    O0SettingsStore *settings = new O0SettingsStore("gm-companion");
    m_o2spotify->setStore(settings);

    // Scopes
    m_o2spotify->setScope("user-library-read playlist-read-private streaming "
                          "user-modify-playback-state user-read-currently-playing "
                          "user-read-playback-state");
    m_o2spotify->setLocalPort(59991);

    // Signals
    connect(m_o2spotify,                  &O2Spotify::linkingSucceeded,   this,                &Spotify::onLinkingSucceeded);
    connect(m_o2spotify,                  &O2Spotify::openBrowser,        this,                &Spotify::onOpenBrowser);
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, &m_librespotProcess, &QProcess::kill);
    connect(&m_librespotProcess,          QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [ = ](int exitCode, QProcess::ExitStatus exitStatus) {
        emit wrongPassword();
        qCCritical(gmSpotify()) << "Librespot exited with code" << exitCode << exitStatus;
    });
}

Spotify::~Spotify()
{
    instanceFlag = false;
    m_o2spotify->deleteLater();
    m_networkManager->deleteLater();
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

void Spotify::grant()
{
    qCDebug(gmSpotify) << "Granting access ...";

    QString id     = m_sManager.getSetting(Setting::spotifyID);
    QString secret = m_sManager.getSetting(Setting::spotifySecret);

    // Client ID and Secret
    m_o2spotify->setClientId(id);
    m_o2spotify->setClientSecret(secret);

    if (!id.isEmpty() && !secret.isEmpty())
    {
        qCDebug(gmSpotify) << "Trying to link ...";
        m_o2spotify->link();
    }
    else
    {
        qCCritical(gmSpotify) << "Client ID or Secret is empty!";
    }
}

int Spotify::get(QNetworkRequest request)
{
    auto requestor = new O2Requestor(m_networkManager, m_o2spotify, this);
    auto requestId = requestor->get(request);

    connect(requestor, qOverload<int, QNetworkReply::NetworkError, QByteArray>(&O2Requestor::finished),
            [ = ](int id, QNetworkReply::NetworkError error, QByteArray data)
    {
        emit(receivedGet(id, error, data));
        requestor->deleteLater();
    });

    return requestId;
}

int Spotify::get(QUrl url)
{
    QNetworkRequest request(url);

    return get(request);
}

int Spotify::put(QUrl url, QString params)
{
    auto requestor = new O2Requestor(m_networkManager, m_o2spotify, this);
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_XFORM);
    int requestId = requestor->put(request, params.toLocal8Bit());

    connect(requestor, qOverload<int, QNetworkReply::NetworkError, QByteArray>(&O2Requestor::finished),
            [ = ](int id, QNetworkReply::NetworkError error, QByteArray data)
    {
        qCDebug(gmSpotify) << "Completed PUT Request. ID:" << id;

        if (error != QNetworkReply::NoError) qWarning() << error;

        qCDebug(gmSpotify) << "Reply:";
        qCDebug(gmSpotify) << data;

        if (error == QNetworkReply::NetworkError::ContentNotFoundError)
        {
            forceCurrentMachine();
        }

        emit receivedPut(id, error, data);
        requestor->deleteLater();
    });

    qCDebug(gmSpotify) << "Sending PUT Request to URL" << url << "and parameters:" << params;
    return requestId;
}

int Spotify::post(QNetworkRequest request, QByteArray data)
{
    auto requestor = new O2Requestor(m_networkManager, m_o2spotify, this);
    auto requestId = requestor->post(request, data);

    connect(requestor, qOverload<int, QNetworkReply::NetworkError, QByteArray>(&O2Requestor::finished),
            [ = ](int id, QNetworkReply::NetworkError error, QByteArray data)
    {
        emit(receivedPost(id, error, data));
        requestor->deleteLater();
    });

    return requestId;
}

void Spotify::forceCurrentMachine()
{
    qCDebug(gmSpotify) << "Setting current machine as active device ...";

    auto requestor = new O2Requestor(m_networkManager, m_o2spotify, this);
    QNetworkRequest request(QUrl("https://api.spotify.com/v1/me/player/devices"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);

    connect(requestor, qOverload<int, QNetworkReply::NetworkError, QByteArray>(&O2Requestor::finished),
            [ = ](int id, QNetworkReply::NetworkError error, QByteArray data)
    {
        requestor->deleteLater();
        onForcedCurrentMachine(id, error, data);
    });

    requestor->get(request);
}

void Spotify::openSpotify()
{
    ProcessInfo pi;

    if (pi.getProcIdByName("librespot") == -1)
    {
        auto username = m_sManager.getSetting(Setting::spotifyUsername);
        auto password = m_sManager.getSetting(Setting::spotifyPassword);

        if (!username.isEmpty() && !password.isEmpty())
        {
            #ifdef Q_OS_LINUX
            m_librespotProcess.start("librespot", { "-n", "GM-Companion",
                                                    "-u", username,
                                                    "-p", password
                                     });
            #endif // ifdef Q_OS_LINUX
            #ifdef Q_OS_WIN
            m_librespotProcess.start("./librespot.exe", { "-n", "GM-Companion",
                                                          "-u", username,
                                                          "-p", password
                                     });
            #endif
        }
    }

    forceCurrentMachine();
}

void Spotify::onLinkingSucceeded()
{
    if (!m_o2spotify->linked()) return;

    openSpotify();
    m_waitingForAuth = false;

    qCDebug(gmSpotify) << "Access has been granted!";
    emit authorized();
}

void Spotify::onOpenBrowser(QUrl url)
{
    m_authUrl        = url;
    m_waitingForAuth = true;
    QDesktopServices::openUrl(url);
    emit authorize(url);
}

void Spotify::onForcedCurrentMachine(int id, QNetworkReply::NetworkError error, QByteArray data)
{
    if (error != QNetworkReply::NoError) qWarning() << "(onForcedCurrentMachine()) ID:" << id << error;

    const auto devices = QJsonDocument::fromJson(data).object().value("devices").toArray();

    qCDebug(gmSpotify) << "Devices:" << devices;

    for (auto device : devices)
    {
        if ((device.toObject().value("name") == "GM-Companion") &&
            !device.toObject().value("is_active").toBool())
        {
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
}
