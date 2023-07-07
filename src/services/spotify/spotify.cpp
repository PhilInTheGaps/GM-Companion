#include "spotify.h"
#include "settings/settingsmanager.h"
#include <QDesktopServices>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmSpotify, "gm.service.spotify")

Spotify::Spotify(QObject *parent)
    : Service(u"Spotify"_s, parent), albums(new AlbumAPI(this)), player(new PlayerAPI(this)),
      playlists(new PlaylistsAPI(this)), tracks(new TracksAPI(this))
{
    m_networkManager = new QNetworkAccessManager(this);
    username(SettingsManager::instance()->get<QString>(u"spotifyUsername"_s, u""_s, u"Spotify"_s));

    connect(m_librespotController.status(), &ServiceStatus::messageChanged, this, &Spotify::forwardClientStatus);

    updateConnector();
}

void Spotify::updateConnector()
{
    qCDebug(gmSpotify()) << "Updating spotify connector ...";

    if (m_connector) m_connector->deleteLater();

    if (SettingsManager::instance()->get(u"connection"_s, u"default"_s, u"Spotify"_s) == "local"_L1)
    {
        m_connector = new SpotifyConnectorLocal(*m_networkManager, new O2Spotify, this);
    }
    else
    {
        m_connector = new SpotifyConnectorServer(*m_networkManager, this);
    }

    connect(m_connector, &RESTServiceConnector::accessGranted, this, &Spotify::onAccessGranted);
    connect(m_connector, &RESTServiceConnector::statusChanged, this, &Spotify::updateStatus);
    connect(m_connector, &RESTServiceConnector::isConnectedChanged, this,
            [this](bool connected) { this->connected(connected); });

    if (connected())
    {
        if (m_librespotController.hasStarted())
        {
            grant();
        }
        else
        {
            m_librespotController.start().then(this, [this](bool success) {
                if (success) grant();
            });
        }
    }
}

auto Spotify::startClient() -> QFuture<bool>
{
    return m_librespotController.start();
}

auto Spotify::instance() -> Spotify *
{
    if (!m_instance)
    {
        m_instance = new Spotify(nullptr);
    }

    return m_instance;
}

void Spotify::grant()
{
    if (!m_connector)
    {
        qCCritical(gmSpotify()) << "grant(): No connector has been initialized!";
        return;
    }

    m_connector->grantAccess();
}

auto Spotify::isGranted() const -> bool
{
    if (!m_connector)
    {
        qCCritical(gmSpotify()) << "isGranted(): No connector has been initialized!";
        return false;
    }

    return m_connector->isAccessGranted();
}

auto Spotify::get(const QNetworkRequest &request) -> QFuture<gsl::owner<RestNetworkReply *>>
{
    if (!connected() || !m_connector) return {};

    const auto callback = [this](RestNetworkReply *reply) {
        handleNetworkError(reply);
        return reply;
    };

    return m_connector->get(request).then(this, callback);
}

auto Spotify::get(const QUrl &url) -> QFuture<gsl::owner<RestNetworkReply *>>
{
    return get(QNetworkRequest(url));
}

auto Spotify::put(const QNetworkRequest &request, const QByteArray &data) -> QFuture<gsl::owner<RestNetworkReply *>>
{
    if (!connected() || !m_connector) return {};

    const auto callback = [this](RestNetworkReply *reply) {
        handleNetworkError(reply);
        return reply;
    };

    return m_connector->put(request, data).then(this, callback);
}

auto Spotify::put(const QUrl &url, const QByteArray &data) -> QFuture<gsl::owner<RestNetworkReply *>>
{
    return put(QNetworkRequest(url), data);
}

auto Spotify::post(const QNetworkRequest &request, const QByteArray &data) -> QFuture<gsl::owner<RestNetworkReply *>>
{
    if (!connected() || !m_connector) return {};

    const auto callback = [this](RestNetworkReply *reply) {
        handleNetworkError(reply);
        return reply;
    };

    return m_connector->post(request, data).then(this, callback);
}

auto Spotify::clientStatus() const -> ServiceStatus *
{
    return m_librespotController.status();
}

void Spotify::connectService()
{
    username(SettingsManager::instance()->get<QString>(u"spotifyUsername"_s, u""_s, u"Spotify"_s));

    auto hasClientStarted = startClient();

    hasClientStarted.then(this, [this](bool success) {
        qCDebug(gmSpotify()) << "Client has started:" << success;

        if (success)
        {
            updateConnector();
            grant();
        }
    });
}

void Spotify::disconnectService()
{
    connected(false);
    if (m_connector) m_connector->disconnectService();
    SettingsManager::setPassword(username(), u""_s, u"Spotify"_s);
    SettingsManager::instance()->set(u"spotifyUsername"_s, u""_s, u"Spotify"_s);
    SettingsManager::instance()->set(u"spotifyID"_s, u""_s, u"Spotify"_s);
    SettingsManager::instance()->set(u"spotifySecret"_s, u""_s, u"Spotify"_s);
    m_librespotController.stop();
}

void Spotify::handleNetworkError(const RestNetworkReply *reply)
{
    if (!reply)
    {
        qCCritical(gmSpotify()) << "Error: trying to handle network error, but reply is null!";
        return;
    }

    if (!reply->hasError()) return;

    switch (reply->error())
    {
    case QNetworkReply::ContentNotFoundError:
        qCDebug(gmSpotify) << "No active spotify device set. Will try to set the current machine.";
        m_librespotController.setAsActiveDevice();
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
    qCWarning(gmSpotify) << "Content Access Denied!" << (QString)error;

    if (error.reason() == "PREMIUM_REQUIRED"_L1)
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

    connected(true);
}

void Spotify::forwardClientStatus(const QString &message)
{
    Q_UNUSED(message)
    emit clientStatusChanged();
}
