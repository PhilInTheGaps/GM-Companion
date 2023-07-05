#pragma once

#include "basespotifyelement.h"
#include "spotifyartist.h"
#include "spotifyimage.h"
#include <QSharedPointer>
#include <QString>

struct SpotifyAlbumInfo : public BaseSpotifyElement
{
    [[nodiscard]] static auto fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyAlbumInfo>;
    [[nodiscard]] static auto fromJson(const QByteArray &data) -> QSharedPointer<SpotifyAlbumInfo>;

    QString releaseDate;

    QList<QSharedPointer<SpotifyImage>> images;
    QList<QSharedPointer<SpotifyArtist>> artists;
};
