#include "spotifyplaylist.h"
#include "spotifyimage.h"
#include "spotifytrack.h"

#include <QJsonDocument>

auto SpotifyPlaylist::fromJson(const QJsonObject &json) -> QSharedPointer<SpotifyPlaylist>
{
    auto *playlist = new SpotifyPlaylist;

    playlist->id              = json[QStringLiteral("id")].toString();
    playlist->href            = json[QStringLiteral("href")].toString();
    playlist->uri             = json[QStringLiteral("uri")].toString();
    playlist->name            = json[QStringLiteral("name")].toString();
    playlist->description     = json[QStringLiteral("description")].toString();
    playlist->snapshotId      = json[QStringLiteral("snapshot_id")].toString();
    playlist->isCollaborative = json[QStringLiteral("collaborative")].toBool();
    playlist->isPublic        = json[QStringLiteral("public")].toBool();
    playlist->images          = SpotifyImage::fromJson(json[QStringLiteral("images")].toArray());

    // TODO: tracks

    return QSharedPointer<SpotifyPlaylist>(playlist);
}

auto SpotifyPlaylist::fromJson(const QByteArray &data) -> QSharedPointer<SpotifyPlaylist>
{
    const auto json = QJsonDocument::fromJson(data).object();
    return SpotifyPlaylist::fromJson(json);
}
