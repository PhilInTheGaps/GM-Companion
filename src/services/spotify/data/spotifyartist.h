#pragma once

#include <QJsonObject>
#include <QJsonArray>
#include <QSharedPointer>
#include "basespotifyelement.h"
#include "spotifyimage.h"

struct SpotifyArtist : public BaseSpotifyElement
{
    [[nodiscard]] static auto fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyArtist>;
    [[nodiscard]] static auto fromJson(const QJsonArray &json) -> QVector<QSharedPointer<SpotifyArtist>>;

    QVector<QSharedPointer<SpotifyImage>> images;
};

