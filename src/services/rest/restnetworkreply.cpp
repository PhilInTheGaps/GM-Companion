#include "restnetworkreply.h"

auto RestNetworkReply::getHeader(const QByteArray &name) const -> QByteArray
{
    const auto headerPairs = headers();

    for (const auto &headerPair : headerPairs)
    {
        if (headerPair.first == name)
        {
            return headerPair.second;
        }
    }

    return {};
}

auto RestNetworkReply::hasError() const -> bool
{
    return error() != QNetworkReply::NoError;
}
