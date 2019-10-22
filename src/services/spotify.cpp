#include "spotify.h"

#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QByteArray>
#include <QUrlQuery>
#include <QHostInfo>
#include <QProcess>

#include "src/utils/processinfo.h"

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
    connect(m_o2spotify, &O2Spotify::linkingSucceeded, this, &Spotify::onLinkingSucceeded);
    connect(m_o2spotify, &O2Spotify::openBrowser,      this, &Spotify::onOpenBrowser);
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
    qDebug() << "SPOTIFY: Granting access ...";

    QString id     = m_sManager.getSetting(Setting::spotifyID);
    QString secret = m_sManager.getSetting(Setting::spotifySecret);

    // Client ID and Secret
    m_o2spotify->setClientId(id);
    m_o2spotify->setClientSecret(secret);

    if (!id.isEmpty() && !secret.isEmpty())
    {
        qDebug() << "SPOTIFY: Trying to link ...";
        m_o2spotify->link();
    }
    else
    {
        qDebug() << "SPOTIFY ERROR: Client ID or Secret is empty!";
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
        qDebug() << "Completed PUT Request. ID:" << id;

        if (error != QNetworkReply::NoError) qWarning() << error;

        qDebug() << "Reply:";
        qDebug() << data;

        if (error == QNetworkReply::NetworkError::ContentNotFoundError)
        {
            forceCurrentMachine();
        }

        emit receivedPut(id, error, data);
        requestor->deleteLater();
    });

    qDebug() << "SPOTIFY: Sending PUT Request to URL" << url << "and parameters:" << params;
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
    qDebug() << "SPOTIFY: Setting current machine as active ...";

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

    if (pi.getProcIdByName("spotify") == -1)
    {
        #ifdef Q_OS_LINUX
        QProcess::startDetached("spotify");
        #endif // ifdef Q_OS_LINUX
    }
}

void Spotify::onLinkingSucceeded()
{
    if (!m_o2spotify->linked()) return;

    openSpotify();
    forceCurrentMachine();
    m_waitingForAuth = false;

    qDebug() << "SPOTIFY: Access has been granted!";
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

    for (auto device : devices)
    {
        if ((device.toObject().value("name") == QHostInfo::localHostName()) &&
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
