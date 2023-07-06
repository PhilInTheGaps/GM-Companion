#include "musicplayer.h"
#include "services/spotify/spotify.h"
#include "services/spotify/spotifyutils.h"
#include "settings/settingsmanager.h"
#include "thirdparty/asyncfuture/asyncfuture.h"
#include "utils/fileutils.h"
#include <QLoggingCategory>
#include <QRandomGenerator>
#include <algorithm>

using namespace Qt::Literals::StringLiterals;
using namespace AsyncFuture;

Q_LOGGING_CATEGORY(gmAudioMusic, "gm.audio.music")

MusicPlayer::MusicPlayer(MetaDataReader &metaDataReader, QObject *parent)
    : AudioPlayer(parent), m_spotifyPlayer(metaDataReader)
{
    m_mediaPlayer.setObjectName(tr("Music"));
    m_mediaPlayer.setAudioOutput(&m_audioOutput);

    connect(&m_spotifyPlayer, &SpotifyPlayer::songEnded, this, &MusicPlayer::onSpotifySongEnded);

    connectPlaybackStateSignals();
    connectMetaDataSignals(metaDataReader);
}

/**
 * @brief Play music list
 * @param name Name of the element
 */
void MusicPlayer::play(AudioElement *element)
{
    if (!element) return;

    qCDebug(gmAudioMusic) << "Playing music list:" << element->name();

    m_mediaPlayer.stop();
    m_mediaPlayer.setObjectName(tr("Music") + ": " + element->name());

    playlistIndex(0);

    m_currentElement = element;
    a_playlist = element->files();

    emit metaDataChanged(&m_mediaPlayer);

    observe(loadPlaylist()).subscribe([this]() { startPlaying(); });
}

/**
 * @brief Start playing music
 */
void MusicPlayer::play()
{
    qCDebug(gmAudioMusic) << "Continue play of MusicPlayer ...";

    switch (m_currentFileSource)
    {
    case AudioFile::Source::Spotify:
        m_spotifyPlayer.play();
        emit startedPlaying();
        break;

    default:
        m_mediaPlayer.play();
        break;
    }
}

void MusicPlayer::startPlaying()
{
    qCDebug(gmAudioMusic) << "startPlaying()";

    if (m_currentElement && !playlist().isEmpty())
    {
        if (m_currentElement->mode() == AudioElement::Mode::Random)
        {
            next();
            return;
        }

        const auto &playlist = this->playlist();
        loadMedia(playlist.first());
    }
}

void MusicPlayer::printPlaylist() const
{
    const auto &playlist = this->playlist();
    for (const auto *audioFile : playlist)
    {
        qCDebug(gmAudioMusic) << audioFile->url();
    }
}

auto MusicPlayer::loadPlaylist() -> QFuture<void>
{
    qCDebug(gmAudioMusic()) << "loadPlaylist()";

    clearPlaylist();

    if (m_playlistLoadingContext) m_playlistLoadingContext->deleteLater();
    m_playlistLoadingContext = new QObject(this);

    return loadPlaylistRecursive();
}

void MusicPlayer::clearPlaylist()
{
    const auto playlist = this->playlist();

    for (auto *entry : playlist)
    {
        if (entry->parent() == this) entry->deleteLater();
    }

    a_playlist.clear();
    emit playlistChanged(a_playlist);
}

void MusicPlayer::loadTrackNamesAsync() const
{
    QList<AudioFile *> spotifyTracks;

    const auto &playlist = this->playlist();
    for (auto *track : playlist)
    {
        if (track->title().isEmpty())
        {
            switch (track->source())
            {
            case AudioFile::Source::Spotify:
                if (SpotifyUtils::getUriType(track->url()) == SpotifyUtils::SpotifyType::Track) spotifyTracks << track;
                break;
            case AudioFile::Source::Youtube:
                track->title(tr("[BROKEN] %1").arg(track->url()));
                qCWarning(gmAudioMusic()) << "Youtube integration is currently broken!";
                break;
            default:
                break;
            }
        }
    }

    loadSpotifyTrackNamesAsync(spotifyTracks);
}

void MusicPlayer::loadSpotifyTrackNamesAsync(const QList<AudioFile *> &files)
{
    if (files.isEmpty()) return;

    QStringList trackIds;
    trackIds.reserve(files.length());

    for (const auto *track : files)
    {
        trackIds << SpotifyUtils::getIdFromUri(track->url());
    }

    const auto callback = [files](const std::vector<QSharedPointer<SpotifyTrack>> &tracks) {
        for (size_t i = 0; i < tracks.size(); i++)
        {
            files[i]->title(tracks[i]->name);
        }
    };

    observe(Spotify::instance()->tracks->getTracks(trackIds)).subscribe(callback);
}

/// Load all entries of a playlist by "expanding" entries like spotify playlists and albums
auto MusicPlayer::loadPlaylistRecursive(int index) -> QFuture<void>
{
    if (index >= m_currentElement->files().length())
    {
        applyShuffleMode();
        loadTrackNamesAsync();
        emit playlistChanged(a_playlist);
        return completed();
    }

    const auto &files = m_currentElement->files();
    auto *audioFile = files[index];

    switch (audioFile->source())
    {
    case AudioFile::Source::Spotify:
        return loadPlaylistRecursiveSpotify(index, audioFile);
    case AudioFile::Source::Youtube:
        qCWarning(gmAudioMusic()) << "Youtube integration is currently broken";
        break;
    default:
        a_playlist.append(audioFile);
        break;
    }

    return loadPlaylistRecursive(index + 1);
}

auto MusicPlayer::loadPlaylistRecursiveSpotify(int index, AudioFile *audioFile) -> QFuture<void>
{
    const auto uri = SpotifyUtils::makeUri(audioFile->url());
    const auto type = SpotifyUtils::getUriType(uri);
    const auto id = SpotifyUtils::getIdFromUri(uri);

    if (SpotifyUtils::isContainerType(type))
    {
        const auto callback = [this, index](const QSharedPointer<SpotifyTrackList> &tracks) {
            for (const auto &track : qAsConst(tracks->tracks))
            {
                if (!track->isPlayable)
                {
                    qCDebug(gmAudioMusic()) << "Spotify track" << track->name << track->album->name << track->uri
                                            << "is not playable -> ignoring it";
                    continue;
                }

                switch (SpotifyUtils::getUriType(track->uri))
                {
                case SpotifyUtils::SpotifyType::Track:
                case SpotifyUtils::SpotifyType::Episode:
                    a_playlist << new AudioFile(track->uri, AudioFile::Source::Spotify, track->name, this);
                default:
                    break;
                }
            }

            return loadPlaylistRecursive(index + 1);
        };

        switch (type)
        {
        case SpotifyUtils::SpotifyType::Playlist:
            return observe(Spotify::instance()->playlists->getPlaylistTracks(id)).subscribe(callback).future();
        case SpotifyUtils::SpotifyType::Album:
            return observe(Spotify::instance()->albums->getAlbumTracks(id)).subscribe(callback).future();
        default:
            qCCritical(gmAudioMusic()) << "loadPlaylistRecursiveSpotify(): not implemented for container type"
                                       << (int)type;
        }
    }

    a_playlist << audioFile;
    return loadPlaylistRecursive(index + 1);
}

/// Shuffle playlist randomly if required
void MusicPlayer::applyShuffleMode()
{
    qCDebug(gmAudioMusic) << "Applying shuffle mode" << m_currentElement->mode();
    if (m_currentElement->mode() != AudioElement::Mode::RandomList) return;

    qCDebug(gmAudioMusic) << "Unshuffled playlist:";
    printPlaylist();

    QList<AudioFile *> temp;

    while (!a_playlist.isEmpty())
    {
        temp.append(a_playlist.takeAt(QRandomGenerator::global()->bounded(a_playlist.size())));
    }
    a_playlist = temp;

    qCDebug(gmAudioMusic) << "Shuffled playlist:";
    printPlaylist();
}

void MusicPlayer::loadMedia(AudioFile *file)
{
    qCDebug(gmAudioMusic) << "Loading media (" << file->url() << ") ...";

    m_mediaPlayer.stop();
    m_spotifyPlayer.stop();
    m_currentFileSource = file->source();
    emit currentIndexChanged();
    emit clearMetaData();

    switch (m_currentFileSource)
    {
    case AudioFile::Source::File:
        loadLocalFile(file);
        break;
    case AudioFile::Source::Web:
        loadWebFile(file);
        break;
    case AudioFile::Source::Spotify:
        loadSpotifyFile(file);
        break;
    case AudioFile::Source::Youtube:
        loadYoutubeFile(file);
        break;
    default:
        qCCritical(gmAudioMusic()) << "loadMedia() not implemented for type" << m_currentFileSource;
        next();
        break;
    }
}

void MusicPlayer::loadLocalFile(AudioFile *file)
{
    if (m_fileRequestContext) m_fileRequestContext->deleteLater();
    m_fileRequestContext = new QObject(this);

    m_fileName = file->url();

    const auto path = FileUtils::fileInDir(file->url(), SettingsManager::getPath(u"music"_s));
    const auto callback = [this](Files::FileDataResult *result) { onFileReceived(result); };

    observe(Files::File::getDataAsync(path)).context(m_fileRequestContext, callback);
}

void MusicPlayer::loadWebFile(AudioFile *file)
{
    m_mediaPlayer.setSource(QUrl(file->url()));
    m_mediaPlayer.play();
    m_audioOutput.setMuted(false);
}

void MusicPlayer::loadSpotifyFile(AudioFile *file)
{
    m_spotifyPlayer.play(file->url());
    emit startedPlaying();
}

void MusicPlayer::loadYoutubeFile(AudioFile *file) const
{
    Q_UNUSED(file)
    qCDebug(gmAudioMusic) << "Media is a youtube video ...";
    qCCritical(gmAudioMusic()) << "Youtube integration is currently broken";
}

/**
 * @brief Pause the music playback
 */
void MusicPlayer::pause()
{
    qCDebug(gmAudioMusic) << "Pausing MusicPlayer ...";

    switch (m_currentFileSource)
    {
    case AudioFile::Source::Spotify:
        m_spotifyPlayer.pause();
        break;

    default:
        m_mediaPlayer.pause();
        break;
    }
}

/**
 * @brief Stop playing music
 */
void MusicPlayer::stop()
{
    qCDebug(gmAudioMusic) << "Stopping MusicPlayer ...";

    m_spotifyPlayer.stop();
    m_mediaPlayer.stop();

    clearPlaylist();
}

void MusicPlayer::next()
{
    emit clearMetaData();

    if (!m_currentElement || a_playlist.isEmpty()) return;

    if (m_currentElement->mode() == AudioElement::Mode::Random)
    {
        playlistIndex(QRandomGenerator::system()->bounded(a_playlist.length()));
    }
    else
    {
        // choose next in line (mode 0, 2, 3)
        if (playlistIndex() < a_playlist.length() - 1)
        {
            playlistIndex(playlistIndex() + 1);
        }

        // loop around (Mode 0 or 2)
        else if (m_currentElement->mode() != AudioElement::Mode::ListOnce)
        {
            playlistIndex(0);
        }
    }

    loadMedia(a_playlist.at(playlistIndex()));
}

void MusicPlayer::again()
{
    switch (m_currentFileSource)
    {
    case AudioFile::Source::Spotify:
        m_spotifyPlayer.again();
        break;

    default:
        m_mediaPlayer.setPosition(0);
        break;
    }
}

void MusicPlayer::setIndex(int index)
{
    playlistIndex(index);

    const auto &playlist = this->playlist();
    loadMedia(playlist[index]);
}

void MusicPlayer::setVolume(int linear, int logarithmic)
{
    m_audioOutput.setVolume(normalizeVolume(logarithmic));
    m_spotifyPlayer.setVolume(linear, logarithmic);
}

void MusicPlayer::onMediaPlayerPlaybackStateChanged(QMediaPlayer::PlaybackState newState)
{
    qCDebug(gmAudioMusic) << "Media player playback state changed:" << newState;
    if (newState == QMediaPlayer::PlayingState) emit startedPlaying();
}

void MusicPlayer::onMediaPlayerMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    switch (status)
    {
    case QMediaPlayer::EndOfMedia:
        qCDebug(gmAudioMusic()) << "End of media was reached, starting next song ...";
        next();
        break;
    case QMediaPlayer::BufferedMedia:
        emit metaDataChanged(&m_mediaPlayer);
        break;
    default:
        break;
    }
}

void MusicPlayer::onMediaPlayerErrorOccurred(QMediaPlayer::Error error, const QString &errorString)
{
    qCWarning(gmAudioMusic()) << error << errorString;

    if (error != QMediaPlayer::NoError)
    {
        next();
    }
}

void MusicPlayer::onFileReceived(Files::FileDataResult *result)
{
    if (!result) return;

    qCDebug(gmAudioMusic()) << "Received file ...";

    // On Windows a there seems to be a weird issue with
    // a strange clicking noise at the beginning of files.
    // Muting the mediaPlayer and unmuting about 100ms into
    // the song seems to be a workaround.
    auto clickingWorkaround = false;

#ifdef Q_OS_WIN
    clickingWorkaround = true;
#endif

    if (clickingWorkaround) m_audioOutput.setMuted(true);
    m_mediaPlayer.stop();

    if (result->data().isEmpty())
    {
        qCWarning(gmAudioMusic()) << "File is empty, skipping ...";
        next();
        result->deleteLater();
        return;
    }

#ifdef Q_OS_WIN
    QFile file(m_tempDir.path() + "/" + FileUtils::fileName(m_fileName));

    if (!file.open(QIODevice::WriteOnly))
    {
        if (file.error() == QFileDevice::OpenError)
        {
            file.setFileName(FileUtils::incrementFileName(file.fileName()));
            if (!file.open(QIODevice::WriteOnly))
            {
                qCWarning(gmAudioMusic()) << "Error: Could not open temporary file even after incrementing the filename"
                                          << file.fileName() << file.errorString();
                result->deleteLater();
                return;
            }
        }
        else
        {
            qCWarning(gmAudioMusic()) << "Error: Could not open temporary file:" << file.fileName()
                                      << file.errorString();
            result->deleteLater();
            return;
        }
    }

    qCDebug(gmAudioMusic()) << file.fileName();
    file.write(result->data());
    file.close();

    m_mediaPlayer.setSource(QUrl::fromLocalFile(file.fileName()));
#else
    m_mediaBuffer.close();
    m_mediaBuffer.setData(result->data());
    m_mediaBuffer.open(QIODevice::ReadOnly);
    m_mediaPlayer.setSourceDevice(&m_mediaBuffer);
#endif

    m_mediaPlayer.play();

    if (clickingWorkaround) QTimer::singleShot(100, this, [this]() { m_audioOutput.setMuted(false); });

    qCDebug(gmAudioMusic()) << "Sending file data to metadatareader ...";

    emit metaDataChanged(result->data());
    result->deleteLater();
}

void MusicPlayer::onSpotifySongEnded()
{
    qCDebug(gmAudioMusic()) << "Spotify song ended, starting next song ...";
    next();
}

void MusicPlayer::connectPlaybackStateSignals() const
{
    connect(&m_mediaPlayer, &QMediaPlayer::playbackStateChanged, this, &MusicPlayer::onMediaPlayerPlaybackStateChanged);
    connect(&m_mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &MusicPlayer::onMediaPlayerMediaStatusChanged);
    connect(&m_mediaPlayer, &QMediaPlayer::errorOccurred, this, &MusicPlayer::onMediaPlayerErrorOccurred);
}

void MusicPlayer::connectMetaDataSignals(MetaDataReader &metaDataReader) const
{
    connect(&m_mediaPlayer, &QMediaPlayer::metaDataChanged, this,
            [this, &metaDataReader]() { metaDataReader.updateMetaData(m_mediaPlayer.metaData()); });

    connect(this, QOverload<const QString &, const QVariant &>::of(&MusicPlayer::metaDataChanged), &metaDataReader,
            QOverload<const QString &, const QVariant &>::of(&MetaDataReader::updateMetaData));

    connect(this, QOverload<const QByteArray &>::of(&MusicPlayer::metaDataChanged), &metaDataReader,
            QOverload<const QByteArray &>::of(&MetaDataReader::updateMetaData));

    connect(this, QOverload<QMediaPlayer *>::of(&MusicPlayer::metaDataChanged), &metaDataReader,
            QOverload<QMediaPlayer *>::of(&MetaDataReader::updateMetaData));

    connect(this, &MusicPlayer::clearMetaData, &metaDataReader, &MetaDataReader::clearMetaData);
}
