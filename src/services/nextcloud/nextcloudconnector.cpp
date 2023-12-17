#include "nextcloudconnector.h"
#include "exceptions/notimplementedexception.h"
#include "nextcloud.h"
#include "settings/settingsmanager.h"
#include "utils/networkutils.h"
#include "utils/stringutils.h"
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QTimer>
#include <chrono>

using namespace Qt::Literals::StringLiterals;
using namespace Services;
using namespace Common::Settings;

constexpr auto NEXTCLOUD_MAX_CONCURRENT_REQUESTS = 3;
constexpr auto AUTH_URL = "/index.php/login/v2";
constexpr auto AUTH_POLL_DELAY = 3000;
constexpr auto MAX_AUTH_POLLS = 20;
constexpr auto TOKEN_VALIDITY_TIME = std::chrono::hours(8766); // 1 year

Q_LOGGING_CATEGORY(gmNcConnector, "gm.service.nextcloud.connector")

NextCloudConnector::NextCloudConnector(NextCloud *nc, QObject *parent)
    : RESTServiceConnector{nullptr, gmNcConnector(), {}, parent}, m_nc(nc)
{
    setMaxConcurrentRequests(NEXTCLOUD_MAX_CONCURRENT_REQUESTS);
    updateTokenExpireTime(TOKEN_VALIDITY_TIME);

    connect(this, &NextCloudConnector::stateChanged, this, &NextCloudConnector::onStateChanged);
    state(State::Connecting);

    QTimer::singleShot(0, this, [this]() {
        m_appPassword = SettingsManager::getPassword(m_nc->loginName(), m_nc->serviceName());

        if (m_nc->loginName().isEmpty() || m_appPassword.isEmpty() || m_nc->serverUrl().isEmpty())
        {
            state(State::Disconnected);
        }
        else
        {
            qCDebug(gmNcConnector()) << "Connected to Nextcloud as user" << m_nc->loginName() << "on server"
                                     << m_nc->serverUrl();
            state(State::Connected);
        }
    });
}

void NextCloudConnector::grantAccess()
{
    qCDebug(gmNcConnector()) << "Logging in ...";

    if (state() == State::Connecting)
    {
        qCDebug(gmNcConnector()) << "Login already in progress ...";
        setStatus(Status::Type::Success, tr("Connected"));
        return;
    }

    if (isAccessGranted())
    {
        qCDebug(gmNcConnector()) << "Already connected to a nextcloud instance.";
    }
    else
    {
        state(State::Connecting);
        startLoginFlow();
    }
}

void NextCloudConnector::disconnectService()
{
    qCDebug(gmNcConnector()) << "Logout() ...";
    setStatus(Status::Type::Info, tr("Logging out ..."));

    SettingsManager::setPassword(m_nc->loginName(), ""_L1, m_nc->serviceName());
    state(State::Disconnected);
}

auto NextCloudConnector::isAccessGranted() const -> bool
{
    return state() == State::Connected;
}

auto NextCloudConnector::get(const QNetworkRequest &request, Options options) -> QFuture<RestReply>
{
    Q_UNUSED(request)
    Q_UNUSED(options)
    throw NotImplementedException();
}

auto NextCloudConnector::put(QNetworkRequest request, const QByteArray &data, Options options) -> QFuture<RestReply>
{
    Q_UNUSED(request)
    Q_UNUSED(data)
    Q_UNUSED(options)
    throw NotImplementedException();
}

auto NextCloudConnector::post(QNetworkRequest request, const QByteArray &data, Options options) -> QFuture<RestReply>
{
    Q_UNUSED(request)
    Q_UNUSED(data)
    Q_UNUSED(options)
    throw NotImplementedException();
}

auto NextCloudConnector::customRequest(const QNetworkRequest &request, const QByteArray &verb, const QByteArray &data,
                                       Options options) -> QFuture<RestReply>
{
    QPromise<RestReply> promise;
    promise.start();

    RestRequest container(request, RestRequest::Type::CUSTOM, options, data, verb);
    return enqueueRequest(std::move(container), std::move(promise));
}

void NextCloudConnector::sendRequest(RestRequest &&container, QPromise<RestReply> &&promise)
{
    auto request = container.request();

    // insert login name if necessary
    if (const auto path = request.url().path().replace("/%251/"_L1, "/%1/"_L1); path.contains("/%1/"_L1))
    {
        auto url = request.url();
        url.setPath(path.arg(m_nc->loginName()), QUrl::TolerantMode);
        request.setUrl(url);
    }

    if (container.options().testFlag(Services::Option::Authenticated))
    {
        request.setRawHeader("Authorization", NetworkUtils::basicAuthHeader(m_nc->loginName(), m_appPassword));
    }

    qCDebug(gmNcConnector) << "Sending" << container.verb() << "Request to URL" << request.url();
    auto *reply = networkManager()->sendCustomRequest(request, container.verb(), container.data());

    auto id = container.id();
    QtFuture::connect(reply, &QNetworkReply::finished).then([this, reply, id]() mutable {
        onReplyReceived(id, reply->error(), reply->errorString(), reply->readAll(), reply->rawHeaderPairs());
        reply->deleteLater();
    });

    markRequestActive(std::move(container), std::move(promise));
}

void NextCloudConnector::refreshAccessToken(bool updateAuthentication)
{
    // nop
    Q_UNUSED(updateAuthentication)
}

auto NextCloudConnector::getAccessToken() -> QString
{
    return m_appPassword;
}

void NextCloudConnector::onStateChanged(State state)
{
    switch (state)
    {
    case NextCloudConnector::State::Connected:
        emit isConnectedChanged(true);
        emit accessGranted();
        dequeueRequests();
        break;
    case NextCloudConnector::State::Disconnected:
        emit isConnectedChanged(false);
        break;
    default:
        break;
    }
}

/**
 * @brief Grant access through the login flow v2:
 * https://docs.nextcloud.com/server/18/developer_manual/client_apis/LoginFlow/index.html#login-flow-v2
 */
void NextCloudConnector::startLoginFlow()
{
    qCDebug(gmNcConnector()) << "Starting login flow v2 ...";

    m_nc->serverUrl(SettingsManager::getServerUrl(m_nc->serviceName(), false));

    if (m_nc->serverUrl().isEmpty())
    {
        setStatus(Status::Type::Error, tr("Error: Server URL is empty."));
        state(State::Disconnected);
        return;
    }

    setStatus(Status::Type::Info, tr("Connecting ..."));
    state(State::Connecting);
    auto authUrl = m_nc->serverUrl() + AUTH_URL;

    qCDebug(gmNcConnector()) << "Server URL:" << m_nc->serverUrl();
    qCDebug(gmNcConnector()) << "Auth URL:" << authUrl;

    auto request = QNetworkRequest(QUrl(authUrl));
    request.setRawHeader("User-Agent", "GM-Companion");

    auto *reply = networkManager()->post(request, "");

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        auto data = QJsonDocument::fromJson(reply->readAll());

        if (reply->error() != QNetworkReply::NoError)
        {
            qCWarning(gmNcConnector()) << "Error:" << reply->error() << reply->errorString();
            setStatus(Status::Type::Error, reply->errorString());
            state(State::Disconnected);
        }
        else
        {
            auto poll = data["poll"_L1].toObject();
            auto token = poll["token"_L1].toString();
            auto endpoint = poll["endpoint"_L1].toString();
            auto login = data["login"_L1].toString();

            setStatus(Status::Type::Info, tr("Waiting for login ..."));
            QDesktopServices::openUrl(QUrl(login));
            pollAuthPoint(QUrl(endpoint), token);
        }

        reply->deleteLater();
    });
}

/**
 * @brief Poll the auth point to receive loginName and appPassword
 */
void NextCloudConnector::pollAuthPoint(const QUrl &url, const QString &token)
{
    qCDebug(gmNcConnector()) << "Polling auth point ...";

    const QByteArray data = "token=" + token.toUtf8();
    auto *reply = networkManager()->post(QNetworkRequest(url), data);
    m_authPolls++;

    qCDebug(gmNcConnector()) << "URL:" << url.toString();
    qCDebug(gmNcConnector()) << "Token:" << StringUtils::censor(token);

    connect(reply, &QNetworkReply::finished, this,
            [this, url, token, reply]() { handleAuthPointReply(reply, url, token); });
}

void NextCloudConnector::handleAuthPointReply(QNetworkReply *reply, const QUrl &url, const QString &token)
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

void NextCloudConnector::handleAuthPointNotFound(const QUrl &url, const QString &token)
{
    if ((m_authPolls < MAX_AUTH_POLLS) && state() == State::Connecting)
    {
        qCDebug(gmNcConnector()) << "Finished poll" << m_authPolls << "/" << MAX_AUTH_POLLS
                                 << "waiting and polling again ...";

        QTimer::singleShot(AUTH_POLL_DELAY, this, [this, url, token]() { pollAuthPoint(url, token); });
    }
    else
    {
        qCWarning(gmNcConnector()) << "Timeout: Max polls (" << MAX_AUTH_POLLS << ") reached!";
        setStatus(Status::Type::Error, tr("Login timed out, please try again."));
        state(State::Disconnected);
    }
}

void NextCloudConnector::handleAuthPointSuccess(QNetworkReply &reply)
{
    const auto content = QJsonDocument::fromJson(reply.readAll()).object();
    m_nc->loginName(content["loginName"_L1].toString());
    m_appPassword = content["appPassword"_L1].toString();

    qCDebug(gmNcConnector()) << "Logged in successfully!";
    qCDebug(gmNcConnector()) << "LoginName:" << m_nc->loginName();
    qCDebug(gmNcConnector()) << "AppPassword:" << StringUtils::censor(m_appPassword);

    SettingsManager::instance()->set("loginName"_L1, m_nc->loginName(), m_nc->serviceName());
    SettingsManager::setPassword(m_nc->loginName(), m_appPassword, m_nc->serviceName());

    state(State::Connected);
}
