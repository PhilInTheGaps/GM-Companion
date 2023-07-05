#pragma once

#include "basespotifyelement.h"
#include "spotifyimage.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QSharedPointer>

struct SpotifyArtist : public BaseSpotifyElement
{
    [[nodiscard]] static auto fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyArtist>;
    [[nodiscard]] static auto fromJson(const QJsonArray &json) -> QList<QSharedPointer<SpotifyArtist>>;

    QList<QSharedPointer<SpotifyImage>> images;
};
