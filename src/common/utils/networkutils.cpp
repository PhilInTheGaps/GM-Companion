#include "networkutils.h"

/**
 * @brief Get a Basic Authorization header from username and password
 */
auto NetworkUtils::basicAuthHeader(const QString &username, const QString &password) -> QByteArray
{
    const auto credentials = QStringLiteral("%1:%2").arg(username, password);
    const auto data        = credentials.toUtf8().toBase64();

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
