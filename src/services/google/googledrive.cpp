#include "googledrive.h"
#include "googledriveconnectorlocal.h"
#include "settings/settingsmanager.h"
#include <QLoggingCategory>
#include <utility>

using namespace Qt::Literals::StringLiterals;

constexpr auto AUTH_REQUEST_URL = "https://accounts.google.com/o/oauth2/auth";
constexpr auto AUTH_TOKEN_URL = "https://accounts.google.com/o/oauth2/token";
constexpr auto AUTH_REFRESH_URL = "https://accounts.google.com/o/oauth2/token";

Q_LOGGING_CATEGORY(gmGoogleDrive, "gm.service.google.drive")

GoogleDrive::GoogleDrive(QQmlEngine &engine, QObject *parent) : GoogleDrive(*engine.networkAccessManager(), parent)
{
}

GoogleDrive::GoogleDrive(QNetworkAccessManager &networkManager, QObject *parent)
    : GoogleDrive(u"Google"_s, networkManager, parent)
{
}

GoogleDrive::GoogleDrive(const QString &serviceName, QNetworkAccessManager &networkManager, QObject *parent)
    : Service(serviceName, parent), m_networkManager(networkManager)
{
    clientId(SettingsManager::instance()->get<QString>(u"googleID"_s, u""_s, serviceName));
    updateConnector();
}

auto GoogleDrive::qmlInstance(QQmlEngine *engine) -> GoogleDrive *
{
    if (s_qmlInstance == nullptr)
    {
        s_qmlInstance = new GoogleDrive(*engine, engine);
    }

    return s_qmlInstance;
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
    clientId(SettingsManager::instance()->get<QString>(u"googleID"_s, u""_s, serviceName()));
    updateConnector();
    grant();
}

void GoogleDrive::disconnectService()
{
    connected(false);
    m_connector->disconnectService();
    SettingsManager::instance()->set(u"googleID"_s, u""_s, serviceName());
    SettingsManager::instance()->set(u"googleSecret"_s, u""_s, serviceName());
}

void GoogleDrive::onAccessGranted()
{
    qCDebug(gmGoogleDrive()) << "Access has been granted.";
    updateStatus(ServiceStatus::Type::Success, tr("Connected"));
    connected(true);
}
