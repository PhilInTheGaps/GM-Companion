#include "nextcloud.h"
#include "logging.h"
#include "settings/settingsmanager.h"
#include "utils/networkutils.h"
#include "thirdparty/asyncfuture/asyncfuture.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QDesktopServices>

using namespace AsyncFuture;

NextCloud::NextCloud(QObject *parent) : Service("NextCloud", parent)
{
    m_networkManager = new QNetworkAccessManager(this);
    m_networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    if (connected())
    {
        updateStatus(ServiceStatus::Type::Success, tr("Connected"));
        loginName(SettingsManager::getSetting("loginName", "", "NextCloud"));
        serverUrl(SettingsManager::getServerUrl("NextCloud", false));
        m_loggingIn = true;

        const auto future = SettingsManager::getPassword(loginName(), "NextCloud");

        observe(future).subscribe([this](const QString &password) {
            m_appPassword = password;

            if (loginName().isEmpty() || m_appPassword.isEmpty() || serverUrl().isEmpty())
            {
                connected(false);
            }
            else
            {
                qCDebug(gmNextCloud()) << "Connected to Nextcloud as user" << loginName() << "on server" << serverUrl();
            }

            m_loggingIn = false;
            emit loggedIn();
        }, [this]() {
            qCCritical(gmNextCloud()) << "Error during reading of password";
            m_loggingIn = false;
            connected(false);
        });
    }
}

auto NextCloud::getInstance() -> NextCloud *
{
    if (!single)
    {
        single = new NextCloud(nullptr);
    }
    return single;
}

auto NextCloud::sendDavRequest(const QByteArray& method, const QString& path, const QByteArray& data,
                               const QList<QPair<QByteArray, QByteArray>>& headers) -> QFuture<QNetworkReply*>
{
    if (!connected()) connectService();

    // In case we are still logging in, delay this request
    if (m_loggingIn)
    {
        qCDebug(gmNextCloud()) << "Delaying DAV request, because we are waiting for authentication ...";

        return observe(this, &NextCloud::loggedIn).subscribe([this, method, path, data]() {
            return sendDavRequest(method, path, data);
        }).future();
    }

    auto url     = getPathUrl(path);
    auto request = QNetworkRequest(QUrl(url));

    qCDebug(gmNextCloud()) << "Sending DAV request (" << method << ") to" << url;

    request.setRawHeader("Authorization", NetworkUtils::basicAuthHeader(loginName(), m_appPassword));

    if (!headers.isEmpty())
    {
        for (const auto &pair : headers)
        {
            if (pair.first == "Destination")
            {
                request.setRawHeader(pair.first, getPathUrl(pair.second).toUtf8());
            }
            else
            {
                request.setRawHeader(pair.first, pair.second);
            }
        }
    }

    return AsyncFuture::completed(m_networkManager->sendCustomRequest(request, method, data));
}

auto NextCloud::getPathUrl(const QString &path) -> QString
{
    const auto seperator = path.startsWith('/') ? QLatin1String() : QStringLiteral("/");
    return QStringLiteral("%1%2/%3%4%5").arg(serverUrl(), DAV_ENDPOINT, loginName(), seperator, path);
}

void NextCloud::connectService()
{
    qCDebug(gmNextCloud()) << "Logging in ...";

    if (m_loggingIn)
    {
        qCDebug(gmNextCloud()) << "Login already in progress ...";
        updateStatus(ServiceStatus::Type::Success, tr("Connected"));
        return;
    }

    if (connected())
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
    updateStatus(ServiceStatus::Type::Info, tr("Logging out ..."));

    // TODO

    connected(false);
}

/**
 * @brief Grant access through the login flow v2:
 * https://docs.nextcloud.com/server/18/developer_manual/client_apis/LoginFlow/index.html#login-flow-v2
 */
void NextCloud::startLoginFlow()
{
    qCDebug(gmNextCloud()) << "Starting login flow v2 ...";

    serverUrl(SettingsManager::getServerUrl("NextCloud", false));

    if (serverUrl().isEmpty())
    {
        updateStatus(ServiceStatus::Type::Error, tr("Error: Server URL is empty."));
        m_loggingIn = false;
        return;
    }

    updateStatus(ServiceStatus::Type::Info, tr("Connecting ..."));
    auto authUrl = serverUrl() + AUTH_URL;

    qCDebug(gmNextCloud()) << "Server URL:" << serverUrl();
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
            updateStatus(ServiceStatus::Type::Error, reply->errorString());
            m_loggingIn = false;
        }
        else
        {
            auto poll     = data.object()["poll"].toObject();
            auto token    = poll["token"].toString();
            auto endpoint = poll["endpoint"].toString();
            auto login    = data.object()["login"].toString();

            updateStatus(ServiceStatus::Type::Info, tr("Waiting for login ..."));
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
            if ((m_authPolls < MAX_AUTH_POLLS) && m_loggingIn)
            {
                qCDebug(gmNextCloud()) << "Finished poll" << m_authPolls << "/"
                                       << MAX_AUTH_POLLS
                                       << "waiting and polling again ...";

                QTimer::singleShot(AUTH_POLL_DELAY, [ = ]() { pollAuthPoint(url, token); });
            }
            else
            {
                qCWarning(gmNextCloud()) << "Timeout: Max polls (" << MAX_AUTH_POLLS << ") reached!";
                updateStatus(ServiceStatus::Type::Error, tr("Login timed out, please try again."));
                m_loggingIn = false;
            }
        }
        else
        {
            auto content = QJsonDocument::fromJson(reply->readAll()).object();
            loginName(content["loginName"].toString());
            m_appPassword = content["appPassword"].toString();

            qCDebug(gmNextCloud()) << "Logged in successfully!";
            qCDebug(gmNextCloud()) << "LoginName:" << loginName();
            qCDebug(gmNextCloud()) << "AppPassword:" << m_appPassword;

            SettingsManager::setSetting("loginName", loginName(), "NextCloud");
            SettingsManager::setPassword(loginName(), m_appPassword, "NextCloud");
            connected(true);
            m_loggingIn = false;
        }

        reply->deleteLater();
    });
}
