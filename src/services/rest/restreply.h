#pragma once

#include <QByteArray>
#include <QList>
#include <QNetworkReply>
#include <QString>
#include <utility>

namespace Services
{

class RestReply
{
public:
    explicit RestReply(QNetworkReply::NetworkError error, const QString &errorText, const QByteArray &data,
                       const QList<QNetworkReply::RawHeaderPair> &headers)
        : m_error(error), m_errorText(errorText), m_data(data), m_headers(headers)
    {
    }

    explicit RestReply(QNetworkReply::NetworkError error, QString &&errorText, QByteArray &&data,
                       const QList<QNetworkReply::RawHeaderPair> &headers)
        : m_error(error), m_errorText(std::move(errorText)), m_data(std::move(data)), m_headers(headers)
    {
    }

    [[nodiscard]] auto getHeader(const QByteArray &name) const -> QByteArray;
    [[nodiscard]] auto headers() const -> QList<QNetworkReply::RawHeaderPair>;

    [[nodiscard]] auto hasError() const -> bool;
    [[nodiscard]] auto error() const -> QNetworkReply::NetworkError;
    [[nodiscard]] auto errorText() const -> QString;

    [[nodiscard]] auto data() const -> QByteArray;

private:
    QNetworkReply::NetworkError m_error;
    QString m_errorText;
    QByteArray m_data;
    QList<QNetworkReply::RawHeaderPair> m_headers;
};

} // namespace Services
