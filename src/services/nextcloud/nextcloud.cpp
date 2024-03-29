#include "nextcloud.h"
#include "settings/settingsmanager.h"
#include "utils/networkutils.h"
#include "utils/stringutils.h"
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QTimer>

using namespace Qt::Literals::StringLiterals;
using namespace Services;

constexpr auto AUTH_URL = "/index.php/login/v2";
constexpr auto DAV_ENDPOINT = "/remote.php/dav/files";
constexpr auto AUTH_POLL_DELAY = 3000;
constexpr auto MAX_AUTH_POLLS = 20;

Q_LOGGING_CATEGORY(gmNextCloud, "gm.service.nextcloud")

NextCloud::NextCloud(const QQmlEngine &engine, QObject *parent) : NextCloud(*engine.networkAccessManager(), parent)
{
}

NextCloud::NextCloud(QNetworkAccessManager &networkManager, QObject *parent)
    : NextCloud(u"NextCloud"_s, networkManager, parent)
{
}

NextCloud::NextCloud(const QString &serviceName, QNetworkAccessManager &networkManager, QObject *parent)
    : Service(serviceName, parent), m_networkManager(networkManager)
{
    if (!connected()) return;

    updateStatus(Status::Type::Success, tr("Connected"));
    loginName(SettingsManager::instance()->get<QString>(u"loginName"_s, u""_s, serviceName));
    serverUrl(SettingsManager::getServerUrl(serviceName, false));
    m_loggingIn = true;

    m_appPassword = SettingsManager::getPassword(loginName(), serviceName);

    if (loginName().isEmpty() || m_appPassword.isEmpty() || serverUrl().isEmpty())
    {
        connected(false);
    }
    else
    {
        qCDebug(gmNextCloud()) << "Connected to Nextcloud as user" << loginName() << "on server" << serverUrl();
        emit loggedIn();
    }

    m_loggingIn = false;
}

auto NextCloud::qmlInstance(QQmlEngine *engine) -> NextCloud *
{
    if (s_qmlInstance == nullptr)
    {
        s_qmlInstance = new NextCloud(*engine, engine);
    }

    return s_qmlInstance;
}

auto NextCloud::sendDavRequest(const QByteArray &method, const QString &path, const QByteArray &data,
                               const QList<std::pair<QByteArray, QByteArray>> &headers) -> QFuture<QNetworkReply *>
{
    if (!connected()) connectService();

    // In case we are still logging in, delay this request
    if (m_loggingIn)
    {
        qCDebug(gmNextCloud()) << "Delaying DAV request, because we are waiting for authentication ...";

        return QtFuture::connect(this, &NextCloud::loggedIn)
            .then([this, method, path, data]() { return sendDavRequest(method, path, data); })
            .unwrap();
    }

    auto url = getPathUrl(path);
    auto request = QNetworkRequest(QUrl(url));

    qCDebug(gmNextCloud()) << "Sending DAV request (" << method << ") to" << url;

    request.setRawHeader("Authorization", NetworkUtils::basicAuthHeader(loginName(), m_appPassword));

    if (!headers.isEmpty())
    {
        for (const auto &[key, value] : headers)
        {
            if (key == "Destination")
            {
                request.setRawHeader(key, getPathUrl(value).toUtf8());
            }
            else
            {
                request.setRawHeader(key, value);
            }
        }
    }

    auto *reply = m_networkManager.sendCustomRequest(request, method, data);
    return QtFuture::connect(reply, &QNetworkReply::finished).then([reply]() { return reply; });
}

auto NextCloud::getPathUrl(const QString &path) const -> QString
{
    const auto seperator = path.startsWith('/') ? u""_s : u"/"_s;
    return u"%1%2/%3%4%5"_s.arg(serverUrl(), DAV_ENDPOINT, loginName(), seperator, path);
}

void NextCloud::connectService()
{
    qCDebug(gmNextCloud()) << "Logging in ...";

    if (m_loggingIn)
    {
        qCDebug(gmNextCloud()) << "Login already in progress ...";
        updateStatus(Status::Type::Success, tr("Connected"));
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
    updateStatus(Status::Type::Info, tr("Logging out ..."));

    SettingsManager::setPassword(loginName(), ""_L1, serviceName());

    connected(false);
}

/**
 * @brief Grant access through the login flow v2:
 * https://docs.nextcloud.com/server/18/developer_manual/client_apis/LoginFlow/index.html#login-flow-v2
 */
void NextCloud::startLoginFlow()
{
    qCDebug(gmNextCloud()) << "Starting login flow v2 ...";

    serverUrl(SettingsManager::getServerUrl(serviceName(), false));

    if (serverUrl().isEmpty())
    {
        updateStatus(Status::Type::Error, tr("Error: Server URL is empty."));
        m_loggingIn = false;
        return;
    }

    updateStatus(Status::Type::Info, tr("Connecting ..."));
    auto authUrl = serverUrl() + AUTH_URL;

    qCDebug(gmNextCloud()) << "Server URL:" << serverUrl();
    qCDebug(gmNextCloud()) << "Auth URL:" << authUrl;

    auto request = QNetworkRequest(QUrl(authUrl));
    request.setRawHeader("User-Agent", "GM-Companion");

    auto *reply = m_networkManager.post(request, "");

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        auto data = QJsonDocument::fromJson(reply->readAll());

        if (reply->error() != QNetworkReply::NoError)
        {
            qCWarning(gmNextCloud()) << "Error:" << reply->error() << reply->errorString();
            updateStatus(Status::Type::Error, reply->errorString());
            m_loggingIn = false;
        }
        else
        {
            auto poll = data.object()["poll"_L1].toObject();
            auto token = poll["token"_L1].toString();
            auto endpoint = poll["endpoint"_L1].toString();
            auto login = data.object()["login"_L1].toString();

            updateStatus(Status::Type::Info, tr("Waiting for login ..."));
            QDesktopServices::openUrl(QUrl(login));
            pollAuthPoint(QUrl(endpoint), token);
        }

        reply->deleteLater();
    });
}

/**
 * @brief Poll the auth point to receive loginName and appPassword
 */
void NextCloud::pollAuthPoint(const QUrl &url, const QString &token)
{
    qCDebug(gmNextCloud()) << "Polling auth point ...";

    const QByteArray data = "token=" + token.toUtf8();
    auto *reply = m_networkManager.post(QNetworkRequest(url), data);
    m_authPolls++;

    qCDebug(gmNextCloud()) << "URL:" << url.toString();
    qCDebug(gmNextCloud()) << "Token:" << StringUtils::censor(token);

    connect(reply, &QNetworkReply::finished, this,
            [this, url, token, reply]() { handleAuthPointReply(reply, url, token); });
}

void NextCloud::handleAuthPointReply(QNetworkReply *reply, const QUrl &url, const QString &token)
{
    if (!reply) return;

    // Polling endpoint returns 404 until authentication is done
    if (reply->error() == QNetworkReply::ContentNotFoundError)
    {
        handleAuthPointNotFound(url, token);
    }
    else
    {
        handleAuthPointSuccess(*reply);
    }

    reply->deleteLater();
}

void NextCloud::handleAuthPointNotFound(const QUrl &url, const QString &token)
{
    if ((m_authPolls < MAX_AUTH_POLLS) && m_loggingIn)
    {
        qCDebug(gmNextCloud()) << "Finished poll" << m_authPolls << "/" << MAX_AUTH_POLLS
                               << "waiting and polling again ...";

        QTimer::singleShot(AUTH_POLL_DELAY, this, [this, url, token]() { pollAuthPoint(url, token); });
    }
    else
    {
        qCWarning(gmNextCloud()) << "Timeout: Max polls (" << MAX_AUTH_POLLS << ") reached!";
        updateStatus(Status::Type::Error, tr("Login timed out, please try again."));
        m_loggingIn = false;
    }
}

void NextCloud::handleAuthPointSuccess(QNetworkReply &reply)
{
    const auto content = QJsonDocument::fromJson(reply.readAll()).object();
    loginName(content["loginName"_L1].toString());
    m_appPassword = content["appPassword"_L1].toString();

    qCDebug(gmNextCloud()) << "Logged in successfully!";
    qCDebug(gmNextCloud()) << "LoginName:" << loginName();
    qCDebug(gmNextCloud()) << "AppPassword:" << StringUtils::censor(m_appPassword);

    SettingsManager::instance()->set("loginName"_L1, loginName(), serviceName());
    SettingsManager::setPassword(loginName(), m_appPassword, serviceName());
    connected(true);
    m_loggingIn = false;
    emit loggedIn();
}
