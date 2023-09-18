#include "abstractmocknetworkmanager.h"
#include <QDebug>

AbstractMockNetworkManager::AbstractMockNetworkManager(const QStringList &hosts, QObject *parent)
    : QNetworkAccessManager{parent}, m_hosts(hosts)
{
}

auto AbstractMockNetworkManager::hosts() const -> QStringList
{
    return m_hosts;
}

void AbstractMockNetworkManager::simulateBrowser(const QUrl &url)
{
    qDebug() << "Simulating browser request to" << url;
    get(QNetworkRequest(url));
}

auto AbstractMockNetworkManager::createRequest(Operation op, const QNetworkRequest &req, QIODevice *outgoingData)
    -> QNetworkReply *
{
    const auto host = req.url().host();

    if (m_hosts.contains(host))
    {
        return sendMockReply(op, req, outgoingData ? outgoingData->readAll() : QByteArray());
    }

    return QNetworkAccessManager::createRequest(op, req, outgoingData);
}

auto AbstractMockNetworkManager::baseUrl(const QUrl &url) -> QString
{
    return QStringLiteral("%1://%2").arg(url.scheme(), url.host());
}
