#include "pipedconnector.h"
#include "exceptions/notimplementedexception.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QRandomGenerator>
#include <chrono>

using namespace Qt::Literals::StringLiterals;
using namespace Services;

constexpr auto PIPED_INSTANCES_API_URL = "https://piped-instances.kavin.rocks";
constexpr auto PIPED_INSTANCES_WIKI_URL =
    "https://raw.githubusercontent.com/wiki/TeamPiped/Piped-Frontend/Instances.md"_L1;
constexpr auto PIPED_PARSER_TABLE_COLUMNS = 5;
constexpr auto PIPED_PARSER_TABLE_HEADER_SIZE = 2;
constexpr auto PIPED_INSTANCE_SELECTION_TIME = std::chrono::hours(1);
constexpr auto MAX_CONCURRENT_REQUESTS = 1;

Q_LOGGING_CATEGORY(gmPiped, "gm.service.youtube.piped")

PipedConnector::PipedConnector(QObject *parent) : RESTServiceConnector{nullptr, gmPiped(), {}, parent}
{
    setMaxConcurrentRequests(MAX_CONCURRENT_REQUESTS);
}

auto PipedConnector::instances() const -> std::vector<PipedInstance>
{
    return m_instances;
}

auto PipedConnector::currentInstance() const -> PipedInstance
{
    return m_currentInstance;
}

void PipedConnector::selectBestInstance()
{
    if (m_instances.empty()) return;

    int withCdn = 0;
    for (const auto &instance : m_instances)
    {
        if (instance.cdn)
        {
            withCdn++;
        }
    }

    const int index = withCdn > 0 ? QRandomGenerator::global()->bounded(withCdn)
                                  : QRandomGenerator::global()->bounded(static_cast<int>(m_instances.size()));

    m_currentInstance = m_instances.at(index);
    updateTokenExpireTime(PIPED_INSTANCE_SELECTION_TIME);

    dequeueRequests();
}

/// Load and parse the list of piped instances from the wiki.
/// Adapted from the piped code itself:
/// https://github.com/TeamPiped/instances-api/blob/main/main.go
auto PipedConnector::fetchInstancesAsync() -> QFuture<bool>
{
    m_instances.clear();

    return fetchInstancesFromApiAsync()
        .then([this](bool result) {
            if (result) return QtFuture::makeReadyFuture(result);

            return fetchInstancesFromWikiAsync();
        })
        .unwrap();
}

void PipedConnector::grantAccess()
{
    // no login required
}

void PipedConnector::disconnectService()
{
    // no login required
}

auto PipedConnector::isAccessGranted() const -> bool
{
    // no login required
    return true;
}

auto PipedConnector::get(const QNetworkRequest &request, bool isAuthRequired, bool lowPriority) -> QFuture<RestReply>
{
    QPromise<RestReply> promise;
    promise.start();

    RestRequest container(request, RestRequest::Type::GET);
    container.isAuthRequired(isAuthRequired);
    return enqueueRequest(std::move(container), std::move(promise), lowPriority);
}

auto PipedConnector::put(QNetworkRequest request, const QByteArray &data, bool lowPriority) -> QFuture<RestReply>
{
    Q_UNUSED(request)
    Q_UNUSED(data)
    Q_UNUSED(lowPriority)
    throw NotImplementedException();
}

auto PipedConnector::post(QNetworkRequest request, const QByteArray &data, bool lowPriority) -> QFuture<RestReply>
{
    Q_UNUSED(request)
    Q_UNUSED(data)
    Q_UNUSED(lowPriority)
    throw NotImplementedException();
}

auto PipedConnector::customRequest(const QNetworkRequest &request, const QByteArray &verb, const QByteArray &data,
                                   bool isAuthRequired, bool lowPriority) -> QFuture<RestReply>
{
    Q_UNUSED(request)
    Q_UNUSED(verb)
    Q_UNUSED(data)
    Q_UNUSED(isAuthRequired)
    Q_UNUSED(lowPriority)
    throw NotImplementedException();
}

void PipedConnector::sendRequest(RestRequest &&container, QPromise<RestReply> &&promise)
{
    if (container.isAuthRequired())
    {
        throw NotImplementedException();
    }

    auto request = container.request();
    request.setUrl(QUrl(m_currentInstance.apiUrl + request.url().toString()));
    QNetworkReply *reply = nullptr;

    switch (container.type())
    {
    case RestRequest::Type::GET:
        qCDebug(gmPiped) << "Sending GET Request to URL" << request.url();
        reply = networkManager()->get(request);
        break;
    default:
        throw NotImplementedException();
    }

    auto id = container.id();
    QtFuture::connect(reply, &QNetworkReply::finished).then([this, reply, id]() mutable {
        onReplyReceived(id, reply->error(), reply->errorString(), reply->readAll(), reply->rawHeaderPairs());
    });

    markRequestActive(std::move(container), std::move(promise));
}

void PipedConnector::refreshAccessToken(bool updateAuthentication)
{
    Q_UNUSED(updateAuthentication)

    // no login required, but we reuse this method to select a new instance
    if (m_instances.empty())
    {
        fetchInstancesAsync().then([this](bool result) {
            if (result) selectBestInstance();
        });
        return;
    }

    selectBestInstance();
}

auto PipedConnector::getAccessToken() -> QString
{
    // there is no access token, but we return the api url as a sign that we can send requests
    return m_currentInstance.apiUrl;
}

auto PipedConnector::fetchInstancesFromApiAsync() -> QFuture<bool>
{
    auto request = QNetworkRequest(QUrl(PIPED_INSTANCES_API_URL));
    auto *reply = networkManager()->get(request);

    return QtFuture::connect(reply, &QNetworkReply::finished).then([this, reply]() {
        if (reply->error() != QNetworkReply::NoError) return false;

        const auto entries = QJsonDocument::fromJson(reply->readAll()).array();

        foreach (const auto &entry, entries)
        {
            m_instances.push_back(PipedInstance{entry["name"_L1].toString(), entry["api_url"_L1].toString(),
                                                entry["locations"_L1].toString(), entry["cdn"_L1].toBool()});
        }

        return !m_instances.empty();
    });
}

auto PipedConnector::fetchInstancesFromWikiAsync() -> QFuture<bool>
{
    auto request = QNetworkRequest(QUrl(PIPED_INSTANCES_WIKI_URL));
    auto *reply = networkManager()->get(request);

    return QtFuture::connect(reply, &QNetworkReply::finished).then([this, reply]() {
        const auto body = reply->readAll();
        const auto lines = body.split('\n');

        int skipped = 0;

        foreach (const auto &line, lines)
        {
            const auto split = line.split('|');

            if (split.length() < PIPED_PARSER_TABLE_COLUMNS) continue;

            // skip the table header
            if (skipped < PIPED_PARSER_TABLE_HEADER_SIZE)
            {
                skipped++;
                continue;
            }

            m_instances.push_back(PipedInstance{split.at(0).trimmed(), split.at(1).trimmed(), split.at(2).trimmed(),
                                                split.at(3).trimmed() == "Yes"});
        }

        return !m_instances.empty();
    });
}
