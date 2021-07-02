#include "discord.h"
#include "logging.h"
#include "services.h"
#include "settings/settingsmanager.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDesktopServices>

bool Discord::instanceFlag = false;
Discord *Discord::single = nullptr;

Discord::Discord(QObject *parent) : Service("Discord", parent)
{
    m_networkManager = new QNetworkAccessManager(this);
    m_networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    setChannel(SettingsManager::getSetting("channel", "", "Discord"));
    setEnabled(SettingsManager::getBoolSetting("enabled", false, "Discord"));

    updateStatus(Service::Info, "");
}

Discord *Discord::getInstance()
{
    if (!instanceFlag)
    {
        single       = new Discord;
        instanceFlag = true;
    }
    return single;
}

Discord::~Discord()
{
    instanceFlag = false;
}

void Discord::post(const QString &endpoint, const QByteArray& data)
{
    auto url = serverUrl() + DISCORD_API_ENDPOINT + endpoint;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    auto *reply = m_networkManager->post(QNetworkRequest(url), data);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() != QNetworkReply::NoError)
        {
            qCWarning(gmDiscord()) << "Error:" << reply->error() << reply->errorString();
        }

        reply->deleteLater();
    });
}

void Discord::post(const QString &endpoint, QHttpMultiPart *multipart)
{
    auto url = serverUrl() + DISCORD_API_ENDPOINT + endpoint;
    auto *reply = m_networkManager->post(QNetworkRequest(url), multipart);

    multipart->setParent(reply);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() != QNetworkReply::NoError)
        {
            qCWarning(gmDiscord()) << "Error:" << reply->error() << reply->errorString();
        }

        reply->deleteLater();
    });
}

void Discord::connectService()
{
    testConnection();
}

void Discord::disconnectService()
{
    // Nothing to do
}

void Discord::testConnection()
{
    updateStatus(Service::Info, tr("Testing connection ..."));

    auto url = serverUrl() + DISCORD_API_ENDPOINT + "/status";
    auto *reply = m_networkManager->get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() != QNetworkReply::NoError)
        {
            updateStatus(Service::Error, reply->errorString());
        }
        else
        {
            auto data = reply->readAll();
            auto root = QJsonDocument::fromJson(data).object();
            auto status = root["status"].toString();

            if (status == "connected")
                updateStatus(Service::Success, tr("Connected."));
            else
                updateStatus(Service::Error, tr("Bot is not connected."));
        }

        reply->deleteLater();
    });
}

void Discord::invite()
{
    QDesktopServices::openUrl(serverUrl() + DISCORD_API_ENDPOINT + "/invite");
}

QString Discord::serverUrl() const
{
    return SettingsManager::getServerUrl("Discord");
}
