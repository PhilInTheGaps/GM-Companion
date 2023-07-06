#include "discord.h"
#include "settings/settingsmanager.h"

#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmDiscord, "gm.service.discord")

Discord::Discord(QObject *parent) : Service("Discord", parent), a_enabled(false)
{
    m_networkManager = new QNetworkAccessManager(this);
    m_networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    channel(SettingsManager::instance()->get<QString>(u"channel"_s, u""_s, u"Discord"_s));
    enabled(SettingsManager::instance()->get(u"enabled"_s, false, u"Discord"_s));

    updateStatus(ServiceStatus::Type::Info, u""_s);
}

auto Discord::getInstance() -> Discord *
{
    if (!single)
    {
        single = new Discord(nullptr);
    }
    return single;
}

void Discord::post(const QString &endpoint, const QByteArray &data)
{
    const auto url = serverUrl() + API_ENDPOINT + endpoint;

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
    auto url = serverUrl() + API_ENDPOINT + endpoint;
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
    updateStatus(ServiceStatus::Type::Info, tr("Testing connection ..."));

    const auto url = serverUrl() + API_ENDPOINT + "/status";
    auto *reply = m_networkManager->get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() != QNetworkReply::NoError)
        {
            updateStatus(ServiceStatus::Type::Error, reply->errorString());
        }
        else
        {
            auto data = reply->readAll();
            auto root = QJsonDocument::fromJson(data).object();
            auto status = root["status"].toString();

            if (status == "connected") updateStatus(ServiceStatus::Type::Success, tr("Connected."));
            else
                updateStatus(ServiceStatus::Type::Error, tr("Bot is not connected."));
        }

        reply->deleteLater();
    });
}

void Discord::invite()
{
    QDesktopServices::openUrl(serverUrl() + API_ENDPOINT + "/invite");
}

auto Discord::serverUrl() -> QString
{
    return SettingsManager::getServerUrl("Discord");
}
