#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>

class AbstractMockNetworkManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit AbstractMockNetworkManager(const QStringList &hosts, QObject *parent = nullptr);

    [[nodiscard]] auto hosts() const -> QStringList;

public slots:
    void simulateBrowser(const QUrl &url);

protected:
    auto createRequest(Operation op, const QNetworkRequest &req, QIODevice *outgoingData) -> QNetworkReply * override;

    virtual auto sendMockReply(Operation op, const QNetworkRequest &req, const QByteArray &data) -> QNetworkReply * = 0;

    static auto baseUrl(const QUrl &url) -> QString;

private:
    QStringList m_hosts;
};
