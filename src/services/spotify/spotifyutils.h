#pragma once

#include <QString>
#include <QObject>

class SpotifyUtils
{
public:
    enum SpotifyType {
        Album,
        Playlist,
        Track,
        Artist,
        Episode,
        Show,
        Unknown = -1
    };

    static auto getUriType(const QString& uri) -> SpotifyType;
    static auto getIdFromUri(const QString &uri) -> QString;
    static auto getIdFromHref(const QString& href) -> QString;

private:
    static constexpr const char* ID_FROM_URI = R"((\/(?!.*\/)(?<=\/)|:(?!.*\/)(?!.*:)(?<=:))([[:alnum:]]+))";
    static constexpr const char* ID_FROM_HREF =R"((albums|playlists|tracks|artists|episodes|shows)\/([[:alnum:]]+))";
};

Q_DECLARE_METATYPE(SpotifyUtils::SpotifyType)
