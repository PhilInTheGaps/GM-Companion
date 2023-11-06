#pragma once

#include "audioplaylist.h"
#include <QFuture>
#include <QNetworkAccessManager>

class ResolvingAudioPlaylist : public AudioPlaylist
{
public:
    explicit ResolvingAudioPlaylist(const QString &settingsId, QNetworkAccessManager &networkManager,
                                    Type type = Type::Undefined);

    auto resolve() -> QFuture<void>;

private:
    auto unwrapEntries() -> QFuture<void>;
    auto unwrapPlaylistFile(qsizetype index, const AudioFile &file) -> QFuture<void>;
    auto unwrapSpotify(qsizetype index, const AudioFile &file) -> QFuture<void>;

    void loadTitles();
    void loadSpotifyTitles(const QList<AudioFile *> &tracks) const;

    static auto isPlaylist(const QString &file) -> bool;

    QNetworkAccessManager &m_networkManager;
    QObject m_fileParent;
    QString m_settingsId;
    bool m_isResolving = false;
};
