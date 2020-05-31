#include "nextcloud.h"
#include "logging.h"
#include "../services.h"
#include "settings/settingsmanager.h"
#include "utils/networkutils.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QDesktopServices>

bool NextCloud::instanceFlag = false;
NextCloud *NextCloud::single = nullptr;

NextCloud::NextCloud(QObject *parent) : Service("NextCloud", parent)
{
    m_networkManager = new QNetworkAccessManager;
    m_networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    if (m_connected)
    {
        updateStatus(StatusType::Success, tr("Connected"));
        setLoginName(SettingsManager::getSetting("loginName", "", "NextCloud"));
        setServerUrl(SettingsManager::getServerUrl("NextCloud", false));
        m_appPassword = SettingsManager::getPassword(m_loginName, "NextCloud");

        if (m_loginName.isEmpty() || m_appPassword.isEmpty() || m_serverUrl.isEmpty())
        {
            setConnected(false);
        }
    }
}

NextCloud * NextCloud::getInstance()
{
    if (!instanceFlag)
    {
        single       = new NextCloud;
        instanceFlag = true;
    }
    return single;
}

NextCloud::~NextCloud()
{
    instanceFlag = false;
    m_networkManager->deleteLater();
}

QNetworkReply * NextCloud::sendDavRequest(const QByteArray& method, const QString& path,
                                          const QByteArray& data, QList<QPair<QByteArray, QByteArray>> headers)
{
    if (!m_connected) connectService();

    auto url     = getPathUrl(path);
    auto request = QNetworkRequest(QUrl(url));

    qCDebug(gmNextCloud()) << "Sending DAV request (" << method << ") to" << url;

    request.setRawHeader("Authorization", NetworkUtils::basicAuthHeader(m_loginName, m_appPassword));

    if (!headers.isEmpty())
    {
        for (const auto &pair : headers)
        {
            request.setRawHeader(pair.first, pair.second);
        }
    }

//    qCDebug(gmNextCloud()) << request;

    return m_networkManager->sendCustomRequest(request, method, data);
}

QString NextCloud::getPathUrl(const QString &path)
{
    return m_serverUrl + NEXTCLOUD_DAV_ENDPOINT + "/" + m_loginName + path;
}

void NextCloud::connectService()
{
    qCDebug(gmNextCloud()) << "Logging in ...";

    if (m_loggingIn)
    {
        qCDebug(gmNextCloud()) << "Login already in progress ...";
        updateStatus(StatusType::Success, tr("Connected"));
        return;
    }

    if (m_connected)
    {
        qCDebug(gmNextCloud()) << "Already connected to a nextcloud instance.";
    }
    else
    {
        m_loggingIn = true;
        startLoginFlow();
    }
}

void NextCloud::disconnectService()
{
    qCDebug(gmNextCloud()) << "Logout() ...";
    updateStatus(StatusType::Info, tr("Logging out ..."));

    // TODO

    setConnected(false);
}

/**
 * @brief Grant access through the login flow v2:
 * https://docs.nextcloud.com/server/18/developer_manual/client_apis/LoginFlow/index.html#login-flow-v2
 */
void NextCloud::startLoginFlow()
{
    qCDebug(gmNextCloud()) << "Starting login flow v2 ...";

    setServerUrl(SettingsManager::getServerUrl("NextCloud", false));

    if (m_serverUrl.isEmpty())
    {
        updateStatus(StatusType::Error, tr("Error: Server URL is empty."));
        m_loggingIn = false;
        return;
    }

    updateStatus(StatusType::Info, tr("Connecting ..."));
    auto authUrl = m_serverUrl + NEXTCLOUD_AUTH_URL;

    qCDebug(gmNextCloud()) << "Server URL:" << m_serverUrl;
    qCDebug(gmNextCloud()) << "Auth URL:" << authUrl;

    auto request = QNetworkRequest(QUrl(authUrl));
    request.setRawHeader("User-Agent", "GM-Companion");

    auto *reply = m_networkManager->post(request, "");

    connect(reply, &QNetworkReply::finished, [ = ]() {
        auto data = QJsonDocument::fromJson(reply->readAll());

        qCDebug(gmNextCloud()) << "Auth reply:" << data.toJson();

        if (reply->error() != QNetworkReply::NoError)
        {
            qCWarning(gmNextCloud()) << "Error:" << reply->error() << reply->errorString();
            updateStatus(StatusType::Error, reply->errorString());
            m_loggingIn = false;
        }
        else
        {
            auto poll     = data.object()["poll"].toObject();
            auto token    = poll["token"].toString();
            auto endpoint = poll["endpoint"].toString();
            auto login    = data.object()["login"].toString();

            updateStatus(StatusType::Info, tr("Waiting for login ..."));
            QDesktopServices::openUrl(QUrl(login));
            pollAuthPoint(QUrl(endpoint), token);
        }

        reply->deleteLater();
    });
}

/**
 * @brief Poll the auth point to receive loginName and appPassword
 */
void NextCloud::pollAuthPoint(const QUrl& url, const QString& token)
{
    qCDebug(gmNextCloud()) << "Polling auth point ...";

    QByteArray data = "token=" + token.toUtf8();
    auto *reply     = m_networkManager->post(QNetworkRequest(url), data);
    m_authPolls++;

    qCDebug(gmNextCloud()) << "URL:" << url.toString();
    qCDebug(gmNextCloud()) << "Token:" << token;

    connect(reply, &QNetworkReply::finished, [ = ]() {
        // Polling endpoint returns 404 until authentication is done
        if (reply->error() == QNetworkReply::ContentNotFoundError)
        {
            if ((m_authPolls < NEXTCLOUD_MAX_AUTH_POLLS) && m_loggingIn)
            {
                qCDebug(gmNextCloud()) << "Finished poll" << m_authPolls << "/"
                                       << NEXTCLOUD_MAX_AUTH_POLLS
                                       << "waiting and polling again ...";

                QTimer::singleShot(NEXTCLOUD_AUTH_POLL_DELAY, [ = ]() { pollAuthPoint(url, token); });
            }
            else
            {
                qCWarning(gmNextCloud()) << "Timeout: Max polls (" << NEXTCLOUD_MAX_AUTH_POLLS << ") reached!";
                updateStatus(StatusType::Error, tr("Login timed out, please try again."));
                m_loggingIn = false;
            }
        }
        else
        {
            auto content = QJsonDocument::fromJson(reply->readAll()).object();
            setLoginName(content["loginName"].toString());
            m_appPassword = content["appPassword"].toString();

            qCDebug(gmNextCloud()) << "Logged in successfully!";
            qCDebug(gmNextCloud()) << "LoginName:" << m_loginName;
            qCDebug(gmNextCloud()) << "AppPassword:" << m_appPassword;

            SettingsManager::setSetting("loginName", m_loginName, "NextCloud");
            SettingsManager::setPassword(m_loginName, m_appPassword, "NextCloud");
            setConnected(true);
            m_loggingIn = false;
        }

        reply->deleteLater();
    });
}
