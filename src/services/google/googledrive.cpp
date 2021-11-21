#include "googledrive.h"

#include <utility>
#include "logging.h"
#include "settings/settingsmanager.h"
#include "thirdparty/asyncfuture/asyncfuture.h"

using namespace AsyncFuture;

GoogleDrive::GoogleDrive(QObject *parent) : Service("Google", parent)
{
    m_networkManager = new QNetworkAccessManager(this);
    m_networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    clientId(SettingsManager::getSetting("googleID", "", "Google"));
    updateConnector();
}

auto GoogleDrive::getInstance() -> GoogleDrive *
{
    if (!single)
    {
        single = new GoogleDrive(nullptr);
    }
    return single;
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

auto GoogleDrive::customRequest(const QNetworkRequest& request, const QByteArray& verb, const QByteArray& data) -> QFuture<RestNetworkReply*>
{
    return m_connector->customRequest(request, verb, data);
}

void GoogleDrive::updateConnector()
{
    qCDebug(gmGoogleDrive()) << "Updating google drive connector ...";

    if (m_connector) m_connector->deleteLater();

    m_connector = new GoogleDriveConnectorLocal(m_networkManager, new O2Google, this);

    connect(m_connector, &RESTServiceConnector::accessGranted, this, &GoogleDrive::onAccessGranted);
    connect(m_connector, &RESTServiceConnector::statusChanged, this, &GoogleDrive::updateStatus);

    if (connected()) grant();
}

void GoogleDrive::connectService()
{
    qCDebug(gmGoogleDrive()) << "Connecting GoogleDrive ...";
    clientId(SettingsManager::getSetting("googleID", "", "Google"));
    updateConnector();
    grant();
}

void GoogleDrive::disconnectService()
{
    connected(false);
    m_connector->disconnectService();
    SettingsManager::setSetting("googleID", "", "Google");
    SettingsManager::setSetting("googleSecret", "", "Google");
}

void GoogleDrive::onAccessGranted()
{
    qCDebug(gmGoogleDrive()) << "Access has been granted.";
    connected(true);
}
