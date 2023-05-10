#include "googledrive.h"

#include "logging.h"
#include "settings/settingsmanager.h"
#include <utility>

using namespace AsyncFuture;

constexpr auto AUTH_REQUEST_URL = "https://accounts.google.com/o/oauth2/auth";
constexpr auto AUTH_TOKEN_URL = "https://accounts.google.com/o/oauth2/token";
constexpr auto AUTH_REFRESH_URL = "https://accounts.google.com/o/oauth2/token";

GoogleDrive::GoogleDrive(QNetworkAccessManager &networkManager, QObject *parent)
    : GoogleDrive(QStringLiteral("Google"), networkManager, parent)
{
}

GoogleDrive::GoogleDrive(const QString &serviceName, QNetworkAccessManager &networkManager, QObject *parent)
    : Service(serviceName, parent), m_networkManager(networkManager)
{
    clientId(SettingsManager::instance()->get<QString>(QStringLiteral("googleID"), QLatin1String(), serviceName));
    updateConnector();
}

auto GoogleDrive::get(const QUrl &url) -> QFuture<RestNetworkReply *>
{
    return get(QNetworkRequest(url));
}

auto GoogleDrive::get(const QNetworkRequest &request) -> QFuture<RestNetworkReply *>
{
    return m_connector->get(request);
}

auto GoogleDrive::put(const QUrl &url, const QByteArray &data) -> QFuture<RestNetworkReply *>
{
    return put(QNetworkRequest(url), data);
}

auto GoogleDrive::put(const QNetworkRequest &request, const QByteArray &data) -> QFuture<RestNetworkReply *>
{
    return m_connector->put(request, data);
}

auto GoogleDrive::post(const QUrl &url, const QByteArray &data) -> QFuture<RestNetworkReply *>
{
    return post(QNetworkRequest(url), data);
}

auto GoogleDrive::post(const QNetworkRequest &request, const QByteArray &data) -> QFuture<RestNetworkReply *>
{
    return m_connector->post(request, data);
}

auto GoogleDrive::customRequest(const QNetworkRequest &request, const QByteArray &verb, const QByteArray &data)
    -> QFuture<RestNetworkReply *>
{
    return m_connector->customRequest(request, verb, data);
}

void GoogleDrive::updateConnector()
{
    qCDebug(gmGoogleDrive()) << "Updating google drive connector ...";

    if (m_connector) m_connector->deleteLater();

    auto *o2 = new O2(this, &m_networkManager);
    o2->setRequestUrl(AUTH_REQUEST_URL);
    o2->setTokenUrl(AUTH_TOKEN_URL);
    o2->setRefreshTokenUrl(AUTH_REFRESH_URL);

    m_connector = new GoogleDriveConnectorLocal(serviceName(), m_networkManager, o2, this);

    connect(m_connector, &RESTServiceConnector::accessGranted, this, &GoogleDrive::onAccessGranted);
    connect(m_connector, &RESTServiceConnector::statusChanged, this, &GoogleDrive::updateStatus);

    if (connected()) grant();
}

void GoogleDrive::connectService()
{
    qCDebug(gmGoogleDrive()) << "Connecting GoogleDrive ...";
    clientId(SettingsManager::instance()->get<QString>(QStringLiteral("googleID"), QLatin1String(), serviceName()));
    updateConnector();
    grant();
}

void GoogleDrive::disconnectService()
{
    connected(false);
    m_connector->disconnectService();
    SettingsManager::instance()->set(QStringLiteral("googleID"), QLatin1String(), serviceName());
    SettingsManager::instance()->set(QStringLiteral("googleSecret"), QLatin1String(), serviceName());
}

void GoogleDrive::onAccessGranted()
{
    qCDebug(gmGoogleDrive()) << "Access has been granted.";
    updateStatus(ServiceStatus::Type::Success, tr("Connected"));
    connected(true);
}
