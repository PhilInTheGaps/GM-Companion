#include "restserviceconnectorlocal.h"
#include "settings/settingsmanager.h"
#include "utils/stringutils.h"
#include <QDesktopServices>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <utility>

using namespace Qt::Literals::StringLiterals;
using namespace Services;
using namespace Common::Settings;

RESTServiceConnectorLocal::RESTServiceConnectorLocal(QNetworkAccessManager *networkManager, O2 *o2,
                                                     const QLoggingCategory &loggingCategory, QObject *parent = nullptr)
    : RESTServiceConnector(networkManager, loggingCategory, {}, parent), m_o2(o2)
{
    m_settingsStore = new O0SettingsStore(u"gm-companion"_s, this);
    m_o2->setParent(this);
    m_o2->setStore(m_settingsStore);

    // Connect signals
    connect(m_o2, &O2::linkingSucceeded, this, &RESTServiceConnectorLocal::onLinkingSucceeded);
    connect(m_o2, &O2::openBrowser, this, &RESTServiceConnectorLocal::onOpenBrowser);
    connect(m_o2, &O2::refreshFinished, this, &RESTServiceConnectorLocal::onRefreshFinished);
}

/**
 * @brief Configure parameters like scope, port etc.
 */
void RESTServiceConnectorLocal::setConfig(RESTServiceLocalConfig config)
{
    qCDebug(m_loggingCategory) << "Configuring rest service connector ...";
    setMaxConcurrentRequests(config.maxConcurrentRequests);
    m_o2->setScope(config.scope);
    m_o2->setLocalPort(config.port);
    m_config = std::move(config);
    m_wasConfigured = true;
}

/**
 * @brief Start the linking process
 */
void RESTServiceConnectorLocal::grantAccess()
{
    setStatus(Status::Type::Info, tr("Connecting..."));

    if (!m_wasConfigured)
    {
        setStatus(Status::Type::Error, tr("Internal Error: Connector was not configured."));
        return;
    }

    auto id = SettingsManager::instance()->get(m_config.idRequest);
    if (id.isEmpty())
    {
        setStatus(Status::Type::Error, tr("Error: No Client ID has been set."));
        return;
    }
    m_o2->setClientId(id);

    auto secret = SettingsManager::instance()->get(m_config.secretRequest);
    if (secret.isEmpty())
    {
        setStatus(Status::Type::Error, tr("Error: No Client Secret has been set."));
        return;
    }
    m_o2->setClientSecret(secret);

    qCDebug(m_loggingCategory) << "Found client id and secret. Trying to link now ...";
    qCDebug(m_loggingCategory) << StringUtils::censor(m_o2->token());
    qCDebug(m_loggingCategory) << StringUtils::censor(m_o2->refreshToken());

    if (m_o2->linked())
    {
        if (m_o2->token().isEmpty())
        {
            m_o2->unlink();
            m_o2->link();
        }
    }
    else
    {
        m_o2->link();
    }
}

void RESTServiceConnectorLocal::disconnectService()
{
    qCDebug(m_loggingCategory) << "Disconnecting ...";
    m_o2->unlink();
}

void RESTServiceConnectorLocal::sendRequest(RestRequest &&container, QPromise<RestReply> &&promise)
{
    if (QThread::currentThread() != this->thread())
    {
        QMetaObject::invokeMethod(
            this,
            [this, container = std::move(container), promise = std::move(promise)]() mutable {
                sendRequest(std::move(container), std::move(promise));
            },
            Qt::ConnectionType::QueuedConnection);
        return;
    }

    auto *requestor = makeRequestor();
    auto request = container.request();

    if (container.options().testFlag(Services::Option::Authenticated))
    {
        // Workaround for google drive requests
        if (!m_config.authHeaderFormat.isEmpty())
        {
            requestor->setAddAccessTokenInQuery(false);
            requestor->setAccessTokenInAuthenticationHTTPHeaderFormat(m_config.authHeaderFormat);
        }
    }
    else
    {
        requestor->setAddAccessTokenInQuery(false);
    }

    switch (container.type())
    {
    case RestRequest::Type::GET:
        qCDebug(m_loggingCategory) << "Sending GET Request to URL" << container.request().url();

        container.id(requestor->get(request));
        break;

    case RestRequest::Type::PUT:
        qCDebug(m_loggingCategory) << "Sending PUT Request to URL" << request.url() << "with data" << container.data();
        container.id(requestor->put(request, container.data()));
        break;

    case RestRequest::Type::POST:
        qCDebug(m_loggingCategory) << "Sending POST Request to URL" << request.url() << "with data" << container.data();
        container.id(requestor->post(request, container.data()));
        qCDebug(m_loggingCategory) << "Sent POST request:" << container.id();
        break;

    case RestRequest::Type::CUSTOM:
        qCDebug(m_loggingCategory) << "Sending HTTP request:" << container.verb() << request.url();
        container.id(requestor->customRequest(request, container.verb(), container.data()));
        break;
    }

    if (container.id() == -1)
    {
        qCWarning(m_loggingCategory) << "Error: could not start requestor!";
        promise.future().cancel();
        return;
    }

    qCDebug(m_loggingCategory) << "internal request id:" << container.id();
    markRequestActive(std::move(container), std::move(promise));
}

auto RESTServiceConnectorLocal::isAccessGranted() const -> bool
{
    return m_o2->linked();
}

auto RESTServiceConnectorLocal::getAccessToken() -> QString
{
    return m_o2->token();
}

void RESTServiceConnectorLocal::refreshAccessToken(bool /*updateAuthentication*/)
{
    if (getAccessToken().isEmpty())
    {
        m_o2->link();
        return;
    }

    m_o2->refresh();
}

auto RESTServiceConnectorLocal::get(const QNetworkRequest &request, Options options) -> QFuture<RestReply>
{
    QPromise<RestReply> promise;
    promise.start();

    RestRequest container(request, RestRequest::Type::GET, options);
    return enqueueRequest(std::move(container), std::move(promise));
}

auto RESTServiceConnectorLocal::put(QNetworkRequest request, const QByteArray &data, Options options)
    -> QFuture<RestReply>
{
    QPromise<RestReply> promise;
    promise.start();

    RestRequest container(request, RestRequest::Type::PUT, options, data);
    return enqueueRequest(std::move(container), std::move(promise));
}

auto RESTServiceConnectorLocal::post(QNetworkRequest request, const QByteArray &data, Options options)
    -> QFuture<RestReply>
{
    QPromise<RestReply> promise;
    promise.start();

    RestRequest container(request, RestRequest::Type::POST, options, data);
    return enqueueRequest(std::move(container), std::move(promise));
}

auto RESTServiceConnectorLocal::customRequest(const QNetworkRequest &request, const QByteArray &verb,
                                              const QByteArray &data, Options options) -> QFuture<RestReply>
{
    QPromise<RestReply> promise;
    promise.start();

    RestRequest container(request, RestRequest::Type::CUSTOM, options, data, verb);
    return enqueueRequest(std::move(container), std::move(promise));
}

auto RESTServiceConnectorLocal::makeRequestor() -> O2Requestor *
{
    auto *requestor = new O2Requestor(networkManager(), m_o2, this);

    connect(requestor,
            QOverload<int, QNetworkReply::NetworkError, QString, QByteArray, QList<QNetworkReply::RawHeaderPair>>::of(
                &O2Requestor::finished),
            this, &RESTServiceConnectorLocal::onReplyReceived);
    connect(requestor, QOverload<int, QNetworkReply::NetworkError, QByteArray>::of(&O2Requestor::finished), requestor,
            &O2Requestor::deleteLater);

    return requestor;
}

void RESTServiceConnectorLocal::onRefreshFinished(QNetworkReply::NetworkError error)
{
    if (error != QNetworkReply::NoError)
    {
        setStatus(Status::Type::Error, tr("Error: Could not refresh token."));
        return;
    }

    qCDebug(m_loggingCategory) << "Refresh Token:" << StringUtils::censor(m_o2->refreshToken());

    updateTokenExpireTime(std::chrono::seconds(m_o2->expires()));
}

void RESTServiceConnectorLocal::onLinkingSucceeded()
{
    if (!m_o2->linked())
    {
        emit isConnectedChanged(false);
        return;
    }

    emit isConnectedChanged(true);
    emit accessGranted();

    updateTokenExpireTime(std::chrono::seconds(m_o2->expires()));

    dequeueRequests();
}

void RESTServiceConnectorLocal::onOpenBrowser(const QUrl &url)
{
    QDesktopServices::openUrl(url);
}
