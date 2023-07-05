#pragma once

#include "basespotifyelement.h"
#include "spotifyartist.h"
#include "spotifyimage.h"
#include "spotifytracklist.h"
#include <QJsonObject>
#include <QSharedPointer>

struct SpotifyAlbum : public BaseSpotifyElement
{
    [[nodiscard]] static auto fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyAlbum>;
    [[nodiscard]] static auto fromJson(const QByteArray &data) -> QSharedPointer<SpotifyAlbum>;

    QString releaseDate;

    QList<QSharedPointer<SpotifyImage>> images;
    QList<QSharedPointer<SpotifyArtist>> artists;

    QSharedPointer<SpotifyTrackList> tracks;
};
