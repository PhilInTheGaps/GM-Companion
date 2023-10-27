#pragma once

#include <QMap>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

namespace Services
{

class CallbackServer : public QObject
{
    Q_OBJECT
public:
    explicit CallbackServer(QObject *parent = nullptr);

    void start(quint16 port, int maxRetries = 3, const QString &successPage = QLatin1String());
    [[nodiscard]] auto isRunning() const -> bool;

    [[nodiscard]] auto getUrl() const -> QString;
    [[nodiscard]] auto getParameters() const -> QMap<QString, QString>;

signals:
    void serverError(const QString &error);
    void serverClosed(bool hasParameters);

private:
    QTcpServer m_server;
    quint16 m_port = 0;
    int m_maxRetries = 3;
    int m_tries = 0;
    QString m_successPage;
    QMap<QString, QString> m_parameters;

    [[nodiscard]] auto buildSuccessReply() const -> QByteArray;
    [[nodiscard]] static auto parseQueryParams(const QByteArray &data) -> QMap<QString, QString>;

    void closeServer(QTcpSocket *socket, bool hasParameters);

private slots:
    void onIncomingConnection();
    void onBytesReady();
};

} // namespace Services
