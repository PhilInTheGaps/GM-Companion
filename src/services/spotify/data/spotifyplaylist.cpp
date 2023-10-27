#include "spotifyplaylist.h"
#include "spotifyimage.h"
#include <QJsonDocument>

using namespace Qt::Literals::StringLiterals;
using namespace Services;

auto SpotifyPlaylist::fromJson(const QJsonObject &json) -> SpotifyPlaylist
{
    SpotifyPlaylist playlist;

    playlist.id = json["id"_L1].toString();
    playlist.href = json["href"_L1].toString();
    playlist.uri = json["uri"_L1].toString();
    playlist.name = json["name"_L1].toString();
    playlist.description = json["description"_L1].toString();
    playlist.snapshotId = json["snapshot_id"_L1].toString();
    playlist.isCollaborative = json["collaborative"_L1].toBool();
    playlist.isPublic = json["public"_L1].toBool();
    playlist.images = SpotifyImage::fromJson(json["images"_L1].toArray());

    // TODO: tracks

    return playlist;
}

auto SpotifyPlaylist::fromJson(const QByteArray &data) -> SpotifyPlaylist
{
    const auto json = QJsonDocument::fromJson(data).object();
    return SpotifyPlaylist::fromJson(json);
}
