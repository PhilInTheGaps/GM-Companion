#pragma once

#include <QMap>
#include <QNetworkRequest>
#include <QString>
#include <QUrl>
#include <QUrlQuery>

class NetworkUtils
{
public:
    [[nodiscard]] static auto basicAuthHeader(const QString &username, const QString &password) -> QByteArray;

    [[nodiscard]] static auto makeJsonRequest(const QUrl &url) -> QNetworkRequest;
    static void makeJsonRequest(QNetworkRequest &request);

    [[nodiscard]] static auto queryToMap(const QUrlQuery &query) -> QMap<QString, QString>;

    [[nodiscard]] static auto isHttpUrl(QStringView url) -> bool;
};
