#pragma once

#include <QObject>
#include <QNetworkReply>

class GoogleDriveReply : public QObject
{
    Q_OBJECT
public:
    explicit GoogleDriveReply(QNetworkReply::NetworkError error,
                              const QByteArray &data,
                              const QList<QNetworkReply::RawHeaderPair> &headers,
                              QObject *parent)
        : QObject(parent), m_error(error), m_data(data), m_headers(headers) {}

    [[nodiscard]] auto error() const -> QNetworkReply::NetworkError
    {
        return m_error;
    };
    [[nodiscard]] auto data() const -> QByteArray
    {
        return m_data;
    }
    [[nodiscard]] auto headers() const -> QList<QNetworkReply::RawHeaderPair>
    {
        return m_headers;
    };

private:
    QNetworkReply::NetworkError m_error;
    QByteArray m_data;
    QList<QNetworkReply::RawHeaderPair> m_headers;
};
