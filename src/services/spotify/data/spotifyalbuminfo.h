#pragma once

#include <QString>
#include <QSharedPointer>
#include "basespotifyelement.h"
#include "spotifyimage.h"
#include "spotifyartist.h"

struct SpotifyAlbumInfo : public BaseSpotifyElement
{
    [[nodiscard]] static auto fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyAlbumInfo>;
    [[nodiscard]] static auto fromJson(const QByteArray &data) -> QSharedPointer<SpotifyAlbumInfo>;

    QString releaseDate;

    QVector<QSharedPointer<SpotifyImage>> images;
    QVector<QSharedPointer<SpotifyArtist>> artists;
};

