#include "restnetworkreply.h"

auto RestNetworkReply::getHeader(const QByteArray &name) const -> QByteArray
{
    for (const auto &headerPair : headers())
    {
        if (headerPair.first == name)
        {
            return headerPair.second;
        }
    }

    return QByteArray();
}
