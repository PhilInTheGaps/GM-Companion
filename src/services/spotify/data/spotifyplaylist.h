#pragma once

#include <QJsonObject>
#include <QList>
#include <QString>

#include "basespotifyelement.h"
#include "spotifyimage.h"
#include "spotifytracklist.h"

struct SpotifyPlaylist : public BaseSpotifyElement
{
    [[nodiscard]] static auto fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyPlaylist>;
    [[nodiscard]] static auto fromJson(const QByteArray &data) -> QSharedPointer<SpotifyPlaylist>;

    QString description;
    QString snapshotId;

    bool isCollaborative;
    bool isPublic;

    QList<QSharedPointer<SpotifyImage>> images;
    QSharedPointer<SpotifyTrackList> tracks;
};
