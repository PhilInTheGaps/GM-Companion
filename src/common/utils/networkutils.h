#ifndef NETWORKUTILS_H
#define NETWORKUTILS_H

#include <QString>
#include <QUrl>
#include <QNetworkRequest>

class NetworkUtils
{
public:
    static auto basicAuthHeader(const QString &username, const QString &password) -> QByteArray;

    static auto makeJsonRequest(const QUrl &url) -> QNetworkRequest;
    static void makeJsonRequest(QNetworkRequest &request);
};

#endif // NETWORKUTILS_H
