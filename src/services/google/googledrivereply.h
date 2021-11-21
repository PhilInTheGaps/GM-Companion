#pragma once

#include <QObject>
#include <QNetworkReply>

class GoogleDriveReply : public QObject
{
    Q_OBJECT
public:
    explicit GoogleDriveReply(const QNetworkReply::NetworkError &error,
                              const QByteArray &data,
                              const QList<QNetworkReply::RawHeaderPair> &headers,
                              QObject *parent)
        : QObject(parent), m_error(error), m_data(data), m_headers(headers) {}

    QNetworkReply::NetworkError error() const { return m_error; };
    QByteArray data() const { return m_data; }
    QList<QNetworkReply::RawHeaderPair> headers() const { return m_headers; };

private:
    QNetworkReply::NetworkError m_error;
    QByteArray m_data;
    QList<QNetworkReply::RawHeaderPair> m_headers;
};
