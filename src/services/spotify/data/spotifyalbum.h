#pragma once

#include <QJsonObject>
#include <QSharedPointer>
#include "basespotifyelement.h"
#include "spotifytracklist.h"
#include "spotifyimage.h"
#include "spotifyartist.h"

struct SpotifyAlbum : public BaseSpotifyElement
{
    [[nodiscard]] static auto fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyAlbum>;
    [[nodiscard]] static auto fromJson(const QByteArray &data) -> QSharedPointer<SpotifyAlbum>;

    QString releaseDate;

    QVector<QSharedPointer<SpotifyImage>> images;
    QVector<QSharedPointer<SpotifyArtist>> artists;

    QSharedPointer<SpotifyTrackList> tracks;
};

