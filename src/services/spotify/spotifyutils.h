#pragma once

#include <QObject>
#include <QString>

class SpotifyUtils
{
    Q_GADGET

public:
    enum class SpotifyType
    {
        Album,
        Playlist,
        Track,
        Artist,
        Episode,
        Show,
        Local,
        Unknown
    };

    Q_ENUM(SpotifyType)

    static auto getUriType(const QString &uri) -> SpotifyType;
    static auto typeFromString(const QString &str) -> SpotifyType;

    static auto getIdFromUri(const QString &uri) -> QString;
    static auto getIdFromHref(const QString &href) -> QString;
    static auto makeUri(const QString &input) -> QString;
    static auto isUri(const QString &input) -> bool;
    static auto isContainerType(SpotifyType type) -> bool;

private:
    static constexpr const char *ID_FROM_URI = R"((\/(?!.*\/)(?<=\/)|:(?!.*\/)(?!.*:)(?<=:))(?'id'[[:alnum:]]+))";
    static constexpr const char *ID_FROM_HREF =
        R"((?'type'albums?|playlists?|tracks?|artists?|episodes?|shows?)\/(?'id'[[:alnum:]]+))";
    static constexpr const char *IS_URI = R"(.+\:.+\:.*)";
};
