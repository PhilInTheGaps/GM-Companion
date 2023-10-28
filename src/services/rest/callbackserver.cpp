#include "callbackserver.h"
#include <QFile>
#include <QLoggingCategory>
#include <QUrlQuery>

using namespace Qt::Literals::StringLiterals;
using namespace Services;

Q_LOGGING_CATEGORY(gmCallbackServer, "gm.service.rest.callbackserver")

CallbackServer::CallbackServer(QObject *parent) : QObject{parent}
{
    connect(&m_server, &QTcpServer::newConnection, this, &CallbackServer::onIncomingConnection);
}

void CallbackServer::start(quint16 port, int maxRetries, const QString &successPage)
{
    m_port = port;
    m_maxRetries = maxRetries;
    m_successPage = successPage;
    m_server.listen(QHostAddress::LocalHost, port);
}

auto CallbackServer::isRunning() const -> bool
{
    return m_server.isListening();
}

auto CallbackServer::getUrl() const -> QString
{
    return u"http://localhost:%1"_s.arg(m_port);
}

auto CallbackServer::getParameters() const -> QMap<QString, QString>
{
    return m_parameters;
}

auto CallbackServer::buildSuccessReply() const -> QByteArray
{
    QByteArray page;

    if (!m_successPage.isEmpty())
    {
        if (QFile f(m_successPage); f.open(QIODevice::ReadOnly))
        {
            page = f.readAll();
        }
    }

    QByteArray reply;
    reply.append("HTTP/1.0 200 OK \r\n");
    reply.append("Content-Type: text/html; charset=\"utf-8\"\r\n");
    reply.append(u"Content-Length: %1\r\n\r\n"_s.arg(page.size()).toLatin1());
    reply.append(page);

    return reply;
}

auto CallbackServer::parseQueryParams(const QByteArray &data) -> QMap<QString, QString>
{
    auto splitGetLine = QString(data).split(u"\r\n"_s).first();
    splitGetLine.remove("GET "_L1);
    splitGetLine.remove("HTTP/1.1"_L1);
    splitGetLine.remove("\r\n"_L1);
    splitGetLine.prepend("http://localhost"_L1);
    QUrl getTokenUrl(splitGetLine);

    QList<std::pair<QString, QString>> tokens;
    QUrlQuery query(getTokenUrl);
    tokens = query.queryItems();

    QMap<QString, QString> queryParams;

    foreach (const auto &tokenPair, tokens)
    {
        auto key = QUrl::fromPercentEncoding(QByteArray().append(tokenPair.first.trimmed().toLatin1()));
        auto value = QUrl::fromPercentEncoding(QByteArray().append(tokenPair.second.trimmed().toLatin1()));
        queryParams.insert(key, value);
    }

    return queryParams;
}

void CallbackServer::onIncomingConnection()
{
    auto const *const socket = m_server.nextPendingConnection();

    connect(socket, &QTcpSocket::readyRead, this, &CallbackServer::onBytesReady, Qt::UniqueConnection);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

void CallbackServer::onBytesReady()
{
    auto *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket)
    {
        qCWarning(gmCallbackServer()) << "onBytesReady: No socket available";
        emit serverError(u"Error: No socket available."_s);
        return;
    }

    socket->write(buildSuccessReply());
    qCDebug(gmCallbackServer()) << "onBytesReady: Sent reply";

    auto data = socket->readAll();
    m_parameters = parseQueryParams(data);

    if (m_parameters.isEmpty())
    {
        if (m_tries < m_maxRetries)
        {
            qCDebug(gmCallbackServer()) << "onBytesReady: No query params found, waiting for more callbacks";
            ++m_tries;
            return;
        }

        m_tries = 0;
        qCWarning(gmCallbackServer()) << "onBytesReady: No query params found, maximum callbacks received";
        closeServer(socket, false);
        return;
    }

    qCDebug(gmCallbackServer()) << "onBytesReady: Query params found, closing server";
    closeServer(socket, true);
}

void CallbackServer::closeServer(QTcpSocket *socket, bool hasParameters)
{
    qCDebug(gmCallbackServer()) << "closeServer: Initiating";
    int port = m_server.serverPort();

    if (socket)
    {
        socket->disconnectFromHost();
    }

    m_server.close();
    qCDebug(gmCallbackServer()) << "closeServer: Closed, no longer listening on port" << port;

    emit serverClosed(hasParameters);
}
