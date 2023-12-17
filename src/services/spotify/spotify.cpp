#include "spotify.h"
#include "settings/settingsmanager.h"
#include "spotifyconnectorlocal.h"
#include "spotifyconnectorserver.h"
#include <QDesktopServices>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QNetworkRequest>

using namespace Qt::Literals::StringLiterals;
using namespace Services;
using namespace Common::Settings;

Q_LOGGING_CATEGORY(gmSpotify, "gm.service.spotify")

Spotify::Spotify(QObject *parent) : Service(u"Spotify"_s, parent)
{
    m_networkManager = new QNetworkAccessManager(this);
    username(SettingsManager::instance()->get<QString>(u"username"_s, u""_s, u"Spotify"_s));

    connect(m_librespotController.status(), &Status::messageChanged, this, &Spotify::forwardClientStatus);

    updateConnector();
}

void Spotify::updateConnector()
{
    qCDebug(gmSpotify()) << "Updating spotify connector ...";

    if (m_connector) m_connector->deleteLater();

    if (SettingsManager::instance()->get(u"connection"_s, u"default"_s, u"Spotify"_s) == "local"_L1)
    {
        m_connector = new SpotifyConnectorLocal(m_networkManager, new O2Spotify, this);
    }
    else
    {
        m_connector = new SpotifyConnectorServer(m_networkManager, this);
    }

    connect(m_connector, &RESTServiceConnector::accessGranted, this, &Spotify::onAccessGranted);
    connect(m_connector, &RESTServiceConnector::statusChanged, this, &Spotify::updateStatus);
    connect(m_connector, &RESTServiceConnector::isConnectedChanged, this,
            [this](bool connected) { this->connected(connected); });

    if (!connected()) return;

    // start client delayed
    QTimer::singleShot(0, this, [this]() {
        if (m_librespotController.hasStarted())
        {
            grant();
        }
        else
        {
            m_librespotController.start().then([this](bool success) {
                if (success) grant();
            });
        }
    });
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

auto Spotify::get(const QNetworkRequest &request, Options options) -> QFuture<RestReply>
{
    if (!connected() || !m_connector) return {};

    const auto callback = [this](RestReply &&reply) {
        handleNetworkError(reply);
        return std::move(reply);
    };

    return m_connector->get(request, options).then(callback);
}

auto Spotify::get(const QUrl &url, Options options) -> QFuture<RestReply>
{
    return get(QNetworkRequest(url), options);
}

auto Spotify::put(const QNetworkRequest &request, const QByteArray &data, Options options) -> QFuture<RestReply>
{
    if (!connected() || !m_connector) return {};

    const auto callback = [this](RestReply &&reply) {
        handleNetworkError(reply);
        return std::move(reply);
    };

    return m_connector->put(request, data, options).then(callback);
}

auto Spotify::put(const QUrl &url, const QByteArray &data, Options options) -> QFuture<RestReply>
{
    return put(QNetworkRequest(url), data, options);
}

auto Spotify::post(const QNetworkRequest &request, const QByteArray &data, Options options) -> QFuture<RestReply>
{
    if (!connected() || !m_connector) return {};

    const auto callback = [this](RestReply &&reply) {
        handleNetworkError(reply);
        return std::move(reply);
    };

    return m_connector->post(request, data, options).then(callback);
}

auto Spotify::clientStatus() const -> Status *
{
    return m_librespotController.status();
}

void Spotify::connectService()
{
    username(SettingsManager::instance()->get<QString>(u"username"_s, u""_s, u"Spotify"_s));

    m_librespotController.start().then([this](bool success) {
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
    SettingsManager::instance()->set(u"username"_s, u""_s, u"Spotify"_s);
    SettingsManager::instance()->set(u"clientId"_s, u""_s, u"Spotify"_s);
    SettingsManager::instance()->set(u"clientSecret"_s, u""_s, u"Spotify"_s);
    m_librespotController.stop();
}

void Spotify::handleNetworkError(const RestReply &reply)
{
    if (!reply.hasError()) return;

    switch (reply.error())
    {
    case QNetworkReply::ContentNotFoundError:
        qCDebug(gmSpotify) << "No active spotify device set. Will try to set the current machine.";
        m_librespotController.setAsActiveDevice();
        break;

    case QNetworkReply::ContentAccessDenied:
        handleAccessDenied(SpotifyNetworkError(reply.data()));
        break;

    default:
        qCWarning(gmSpotify) << "A network error occurred:" << reply.error() << reply.errorText() << reply.data();
    }
}

void Spotify::handleAccessDenied(const SpotifyNetworkError &error)
{
    qCWarning(gmSpotify) << "Content Access Denied!" << (QString)error;

    if (error.reason() == "PREMIUM_REQUIRED"_L1)
    {
        connected(false);
        updateStatus(Status::Type::Error, tr("Error: Spotify premium is required!"));
    }
    else
    {
        updateStatus(Status::Type::Error, error.message());
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
