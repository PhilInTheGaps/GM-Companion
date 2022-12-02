#include "spotifynetworkerror.h"

#include <QJsonDocument>
#include <QJsonObject>

SpotifyNetworkError::SpotifyNetworkError(const QByteArray &data)
{
    auto error = QJsonDocument::fromJson(data).object()[QStringLiteral("error")].toObject();
    a_message = error[QStringLiteral("message")].toString();
    a_reason = error[QStringLiteral("reason")].toString();
}

SpotifyNetworkError::operator QString() const
{
    return QStringLiteral("%1 (%2)").arg(message(), reason());
}
