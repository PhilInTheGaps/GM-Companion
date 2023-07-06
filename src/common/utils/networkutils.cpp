#include "networkutils.h"

using namespace Qt::Literals::StringLiterals;

/**
 * @brief Get a Basic Authorization header from username and password
 */
auto NetworkUtils::basicAuthHeader(const QString &username, const QString &password) -> QByteArray
{
    const auto credentials = u"%1:%2"_s.arg(username, password);
    const auto data = credentials.toUtf8().toBase64();

    return "Basic " + data;
}

auto NetworkUtils::makeJsonRequest(const QUrl &url) -> QNetworkRequest
{
    QNetworkRequest request(url);
    makeJsonRequest(request);
    return request;
}

void NetworkUtils::makeJsonRequest(QNetworkRequest &request)
{
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
}

auto NetworkUtils::queryToMap(const QUrlQuery &query) -> QMap<QString, QString>
{
    QMap<QString, QString> map;
    const auto itemPairs = query.queryItems();

    for (const auto &pair : itemPairs)
    {
        map.insert(pair.first, pair.second);
    }

    return map;
}
