#include "nextcloud.h"
#include "settings/settingsmanager.h"
#include <QLoggingCategory>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>

using namespace Qt::Literals::StringLiterals;
using namespace Services;
using namespace Common::Settings;

constexpr auto DAV_ENDPOINT = "/remote.php/dav/files";

Q_LOGGING_CATEGORY(gmNextCloud, "gm.service.nextcloud")

NextCloud::NextCloud(const QQmlEngine &engine, QObject *parent) : NextCloud(engine.networkAccessManager(), parent)
{
}

NextCloud::NextCloud(QNetworkAccessManager *networkManager, QObject *parent)
    : NextCloud(u"NextCloud"_s, networkManager, parent)
{
}

NextCloud::NextCloud(const QString &serviceName, QNetworkAccessManager *networkManager, QObject *parent)
    : Service(serviceName, parent), m_connector(this)
{
    m_connector.setNetworkManager(networkManager);
    connect(&m_connector, &RESTServiceConnector::statusChanged, this, &NextCloud::updateStatus);
    connect(&m_connector, &NextCloudConnector::stateChanged, this, &NextCloud::onConnectorStateChanged);

    if (!connected()) return;

    updateStatus(Status::Type::Success, tr("Connected"));
    loginName(SettingsManager::instance()->get<QString>(u"loginName"_s, u""_s, serviceName));
    serverUrl(SettingsManager::getServerUrl(serviceName, false));

    QTimer::singleShot(0, this, [this]() {
        if (m_connector.state() == NextCloudConnector::State::Connected) emit loggedIn();
    });
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
                               const QList<std::pair<QByteArray, QByteArray>> &headers, Options options)
    -> QFuture<RestReply>
{
    if (QThread::currentThread() != this->thread())
    {
        QPromise<RestReply> promise;
        auto future = promise.future();

        QMetaObject::invokeMethod(
            this,
            [this, method, path, data, headers, options, promise = std::move(promise)]() mutable {
                sendDavRequest(method, path, data, headers, options)
                    .then([promise = std::move(promise)](RestReply &&reply) mutable {
                        promise.addResult(std::move(reply));
                        promise.finish();
                    });
            },
            Qt::ConnectionType::QueuedConnection);
        return future;
    }

    if (!connected()) connectService();

    auto url = getPathUrl(path);
    auto request = QNetworkRequest(QUrl(url));

    qCDebug(gmNextCloud()) << "Sending DAV request (" << method << ") to" << url;

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

    return m_connector.customRequest(request, method, data, Option::Authenticated | options);
}

auto NextCloud::getPathUrl(const QString &path) const -> QString
{
    const auto seperator = path.startsWith('/') ? u""_s : u"/"_s;
    // If loginName is empty (because we are not connected yet), add a placeholder.
    // The name will be inserted in the connector during sendRequest()
    return u"%1%2/%3%4%5"_s.arg(serverUrl(), DAV_ENDPOINT, loginName().isEmpty() ? "%1"_L1 : loginName(), seperator,
                                path);
}

void NextCloud::connectService()
{
    m_connector.grantAccess();
}

void NextCloud::disconnectService()
{
    m_connector.disconnectService();
}

void NextCloud::onConnectorStateChanged(NextCloudConnector::State state)
{
    qCDebug(gmNextCloud()) << "Connector state changed:" << static_cast<int>(state);

    switch (state)
    {
    case NextCloudConnector::State::Connected:
        connected(true);
        emit loggedIn();
        break;
    case NextCloudConnector::State::Disconnected:
        connected(false);
        break;
    default:
        break;
    }
}
