#include "restreply.h"

using namespace Services;

auto RestReply::getHeader(const QByteArray &name) const -> QByteArray
{
    foreach (const auto &headerPair, headers())
    {
        if (headerPair.first == name)
        {
            return headerPair.second;
        }
    }

    return {};
}

auto RestReply::headers() const -> QList<QNetworkReply::RawHeaderPair>
{
    return m_headers;
}

auto RestReply::hasError() const -> bool
{
    return error() != QNetworkReply::NoError;
}

auto RestReply::error() const -> QNetworkReply::NetworkError
{
    return m_error;
}

auto RestReply::errorText() const -> QString
{
    return m_errorText;
}

auto RestReply::data() const -> QByteArray
{
    return m_data;
}
