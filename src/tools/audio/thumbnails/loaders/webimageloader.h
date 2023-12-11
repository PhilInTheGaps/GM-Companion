#pragma once

#include <QFuture>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QString>

class WebImageLoader
{
public:
    static auto loadImageAsync(const QString &url, QNetworkAccessManager *networkManager) -> QFuture<QPixmap>;

private:
    static auto loadImageAsync(QNetworkReply *reply, const QString &url) -> QFuture<QPixmap>;
};
