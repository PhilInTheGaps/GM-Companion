#include "spotifynetworkerror.h"

#include <QJsonDocument>
#include <QJsonObject>

SpotifyNetworkError::SpotifyNetworkError(const QByteArray &data)
{
    auto error = QJsonDocument::fromJson(data).object()["error"].toObject();
    a_message = error["message"].toString();
    a_reason = error["reason"].toString();
}

SpotifyNetworkError::operator QString() const
{
    return QString("%1 (%2)").arg(message(), reason());
}
