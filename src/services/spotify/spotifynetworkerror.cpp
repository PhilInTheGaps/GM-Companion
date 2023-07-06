#include "spotifynetworkerror.h"
#include <QJsonDocument>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;

SpotifyNetworkError::SpotifyNetworkError(const QByteArray &data)
{
    auto error = QJsonDocument::fromJson(data).object()["error"_L1].toObject();
    a_message = error["message"_L1].toString();
    a_reason = error["reason"_L1].toString();
}

SpotifyNetworkError::operator QString() const
{
    return u"%1 (%2)"_s.arg(message(), reason());
}
