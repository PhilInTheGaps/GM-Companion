#include "googledrive.h"

#include <utility>
#include "logging.h"
#include "settings/settingsmanager.h"

bool GoogleDrive::instanceFlag   = false;
GoogleDrive *GoogleDrive::single = nullptr;

GoogleDrive::GoogleDrive(QObject *parent) : Service("Google", parent)
{
    m_networkManager = new QNetworkAccessManager;
    setClientId(SettingsManager::getSetting("googleID", "", "Google"));
    updateConnector();
}

auto GoogleDrive::getInstance()->GoogleDrive *
{
    if (!instanceFlag)
    {
        single       = new GoogleDrive;
        instanceFlag = true;
    }
    return single;
}

GoogleDrive::~GoogleDrive()
{
    instanceFlag = false;
    m_connector->deleteLater();
    m_networkManager->deleteLater();
}

void GoogleDrive::updateConnector()
{
    qCDebug(gmGoogleDrive()) << "Updating google drive connector ...";

    delete m_connector;

    m_connector = new GoogleDriveConnectorLocal(m_networkManager, new O2Google, this);

    connect(m_connector, &RESTServiceConnector::accessGranted, this, &GoogleDrive::onAccessGranted);
    connect(m_connector, &RESTServiceConnector::receivedReply, this, &GoogleDrive::onReceivedReply);
    connect(m_connector, &RESTServiceConnector::statusChanged, this, &GoogleDrive::updateStatus);

    if (connected()) grant();
}

auto GoogleDrive::customRequest(const QNetworkRequest& request, const QByteArray& verb, const QByteArray& data)->int
{
    auto requestId = getUniqueRequestId();

    m_connector->customRequest(request, verb, data, requestId);
    return requestId;
}

void GoogleDrive::connectService()
{
    qCDebug(gmGoogleDrive()) << "Connecting GoogleDrive ...";
    setClientId(SettingsManager::getSetting("googleID", "", "Google"));
    updateConnector();
    grant();
}

void GoogleDrive::disconnectService()
{
    setConnected(false);
    m_connector->disconnectService();
    SettingsManager::setSetting("googleID", "", "Google");
    SettingsManager::setSetting("googleSecret", "", "Google");
}

void GoogleDrive::onAccessGranted()
{
    qCDebug(gmGoogleDrive()) << "Access has been granted.";
    setConnected(true);
}

void GoogleDrive::onReceivedReply(int id, QNetworkReply::NetworkError error, const QByteArray& data, QList<QNetworkReply::RawHeaderPair>headers)
{
    if (error != QNetworkReply::NoError) {
        qCCritical(gmGoogleDrive()) << id << error << data;
    }

    emit receivedReply(id, error, data, std::move(headers));
}
