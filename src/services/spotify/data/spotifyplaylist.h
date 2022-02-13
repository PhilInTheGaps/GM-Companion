#pragma once

#include <QString>
#include <QList>
#include <QJsonObject>

#include "basespotifyelement.h"
#include "spotifytracklist.h"
#include "spotifyimage.h"

struct SpotifyPlaylist : public BaseSpotifyElement
{
    [[nodiscard]] static auto fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyPlaylist>;
    [[nodiscard]] static auto fromJson(const QByteArray &data) -> QSharedPointer<SpotifyPlaylist>;

    QString description;
    QString snapshotId;

    bool isCollaborative;
    bool isPublic;

    QVector<QSharedPointer<SpotifyImage>> images;
    QSharedPointer<SpotifyTrackList> tracks;
};

