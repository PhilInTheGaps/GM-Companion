#include "resolvingaudioplaylist.h"
#include "audioplaylistfactory.h"
#include "filesystem/file.h"
#include "filesystem/results/filedataresult.h"
#include "services/spotify/spotify.h"
#include "services/spotify/spotifyutils.h"
#include "settings/settingsmanager.h"
#include "utils/fileutils.h"
#include <QLoggingCategory>

using namespace Services;
using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmAudioPlaylistResolving, "gm.audio.playlist.resolving")

ResolvingAudioPlaylist::ResolvingAudioPlaylist(const QString &settingsId, QNetworkAccessManager &networkManager,
                                               Type type)
    : AudioPlaylist(type), m_networkManager(networkManager), m_settingsId(settingsId)
{
}

auto ResolvingAudioPlaylist::resolve() -> QFuture<void>
{
    if (m_isResolving) return {};

    m_isResolving = true;

    return unwrapEntries().then([this]() {
        m_isResolving = false;
        loadTitles();
    });
}

auto ResolvingAudioPlaylist::unwrapEntries() -> QFuture<void>
{
    QList<QFuture<void>> futures;
    const AudioFile *audioFile = nullptr;

    for (qsizetype i = 0; i < length(); i++)
    {
        audioFile = at(i);
        if (!audioFile) continue;

        switch (audioFile->source())
        {
        case AudioFile::Source::Spotify:
            futures << unwrapSpotify(i, *audioFile);
            break;
        case AudioFile::Source::File:
        case AudioFile::Source::Web:
            if (isPlaylist(audioFile->url()))
            {
                futures << unwrapPlaylistFile(i, *audioFile);
            }
            break;
        default:
            break;
        }
    }

    return QtFuture::whenAll(futures.begin(), futures.end()).then([](const QList<QFuture<void>> &) {
        // empty continuation to return void
    });
}

auto ResolvingAudioPlaylist::unwrapPlaylistFile(qsizetype index, const AudioFile &file) -> QFuture<void>
{
    auto callback = [this, index](const QByteArray &data) {
        if (data.isEmpty())
        {
            qCWarning(gmAudioPlaylistResolving()) << "Error: File is empty!";
            return;
        }

        auto playlist = AudioPlaylistFactory::build(data, &m_fileParent);
        if (playlist->isEmpty())
        {
            qCWarning(gmAudioPlaylistResolving()) << "Playlist is empty";
            return;
        }

        replace(index, *playlist);
    };

    switch (file.source())
    {
    case AudioFile::Source::File: {
        const auto path = FileUtils::fileInDir(file.url(), SettingsManager::getPath(m_settingsId));
        return Files::File::getDataAsync(path).then(
            [callback](const Files::FileDataResult &result) { callback(result.data()); });
        break;
    }
    case AudioFile::Source::Web: {
        auto *reply = m_networkManager.get(QNetworkRequest(QUrl(file.url())));
        return QtFuture::connect(reply, &QNetworkReply::finished).then([reply, callback]() {
            const auto &data = reply->readAll();
            reply->deleteLater();
            callback(data);
        });
        break;
    }
    default:
        qCWarning(gmAudioPlaylistResolving())
            << "Could not expand playlist file" << file.url() << "with source" << file.source();
        return QtFuture::makeReadyFuture();
    }
}

auto ResolvingAudioPlaylist::unwrapSpotify(qsizetype index, const AudioFile &file) -> QFuture<void>
{
    const auto uri = SpotifyUtils::makeUri(file.url());
    const auto type = SpotifyUtils::getUriType(uri);
    const auto id = SpotifyUtils::getIdFromUri(uri);

    if (!SpotifyUtils::isContainerType(type)) return QtFuture::makeReadyFuture();

    const auto callback = [this, index](const SpotifyTrackList &tracklist) {
        QList<AudioFile *> files;
        foreach (const auto &track, tracklist.tracks)
        {
            if (!track.isPlayable)
            {
                qCDebug(gmAudioPlaylistResolving()) << "Spotify track" << track.name << track.album.name << track.uri
                                                    << "is not playable -> ignoring it";
                continue;
            }

            switch (SpotifyUtils::getUriType(track.uri))
            {
            case SpotifyUtils::SpotifyType::Track:
            case SpotifyUtils::SpotifyType::Episode:
                files << new AudioFile(track.uri, AudioFile::Source::Spotify, track.name, &m_fileParent);
            default:
                break;
            }
        }
        replace(index, files);
    };

    switch (type)
    {
    case SpotifyUtils::SpotifyType::Playlist:
        return Spotify::instance()->playlists.getPlaylistTracks(id).then(callback);
    case SpotifyUtils::SpotifyType::Album:
        return Spotify::instance()->albums.getAlbumTracks(id).then(callback);
    default:
        qCCritical(gmAudioPlaylistResolving())
            << "loadPlaylistRecursiveSpotify(): not implemented for container type" << (int)type;
        return QtFuture::makeReadyFuture();
    }
}

void ResolvingAudioPlaylist::loadTitles()
{
    QList<AudioFile *> spotifyTracks;

    foreach (auto *audioFile, files())
    {
        if (!audioFile || !audioFile->title().isEmpty()) continue;

        switch (audioFile->source())
        {
        case AudioFile::Source::Spotify:
            if (SpotifyUtils::getUriType(audioFile->url()) == SpotifyUtils::SpotifyType::Track)
                spotifyTracks.append(audioFile);
            break;
        case AudioFile::Source::Youtube:
            audioFile->title(QObject::tr("[BROKEN] %1").arg(audioFile->url()));
            break;
        default:
            break;
        }
    }

    loadSpotifyTitles(spotifyTracks);
}

void ResolvingAudioPlaylist::loadSpotifyTitles(const QList<AudioFile *> &tracks) const
{
    if (tracks.isEmpty()) return;

    QStringList trackIds;
    trackIds.reserve(tracks.length());

    foreach (const auto *track, tracks)
    {
        trackIds << SpotifyUtils::getIdFromUri(track->url());
    }

    const auto callback = [tracks](const std::vector<SpotifyTrack> &results) {
        for (size_t i = 0; i < results.size(); i++)
        {
            tracks.at(i)->title(results.at(i).name);
        }
    };

    Spotify::instance()->tracks.getTracks(trackIds).then(callback);
}

auto ResolvingAudioPlaylist::isPlaylist(const QString &file) -> bool
{
    return file.endsWith(".m3u"_L1) || file.endsWith(".m3u8"_L1) || file.endsWith(".pls"_L1);
}
