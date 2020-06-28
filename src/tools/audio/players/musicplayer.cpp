#include "musicplayer.h"
#include "logging.h"
#include "settings/settingsmanager.h"
#include "filesystem/filemanager.h"
#include "utils/fileutils.h"

#include <algorithm>
#include <random>
#include <cstdlib>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
# include <QRandomGenerator>
#endif // if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))

using namespace YouTube::Videos;

MusicPlayer::MusicPlayer(SpotifyPlayer *spotify, DiscordPlayer *discordPlayer, QObject *parent)
    : AudioPlayer(parent), spotifyPlayer(spotify), discordPlayer(discordPlayer)
{
    mediaPlayer = new QMediaPlayer;
    mediaPlayer->setObjectName(tr("Music"));

    auto *networkManager = new QNetworkAccessManager(this);
    networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
    videoClient = new VideoClient(networkManager, this);

    connect(mediaPlayer,                &QMediaPlayer::stateChanged,                              this, &MusicPlayer::onMediaPlayerStateChanged);
    connect(mediaPlayer,                &QMediaPlayer::bufferStatusChanged,                       this, &MusicPlayer::onMediaPlayerBufferStatusChanged);
    connect(mediaPlayer,                &QMediaPlayer::mediaStatusChanged,                        this, &MusicPlayer::onMediaPlayerMediaStatusChanged);
    connect(mediaPlayer,                QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &MusicPlayer::onMediaPlayerError);
    connect(mediaPlayer,                &QMediaPlayer::mediaChanged,                              this, &MusicPlayer::onMediaPlayerMediaChanged);
    connect(mediaPlayer,                QOverload<const QString&, const QVariant&>::of(&QMediaObject::metaDataChanged),
            this, QOverload<const QString&, const QVariant&>::of(&MusicPlayer::metaDataChanged));

    connect(FileManager::getInstance(), &FileManager::receivedFile,              this, &MusicPlayer::onFileReceived);

    connect(spotify,                    &SpotifyPlayer::songEnded,               this, &MusicPlayer::onSpotifySongEnded);
    connect(spotify,                    &SpotifyPlayer::receivedPlaylistTracks,  this, &MusicPlayer::onSpotifyReceivedPlaylistTracks);
}

MusicPlayer::~MusicPlayer()
{
    mediaPlayer->deleteLater();
}

/**
 * @brief Play music list
 * @param name Name of the element
 */
void MusicPlayer::play(AudioElement *element)
{
    if (!element) return;

    qCDebug(gmAudioMusic) << "Playing music list:" << element->name();

    mediaPlayer->stop();
    mediaPlayer->setObjectName(tr("Music") + ": " + element->name());
    m_playlist.clear();
    m_playlistIndex  = 0;
    m_waitingForUrls = 0;

    currentElement = element;

    m_playlist = element->files();

    emit metaDataChanged(mediaPlayer);
    loadSongNames(true);
}

/**
 * @brief Start playing music
 */
void MusicPlayer::play()
{
    qCDebug(gmAudioMusic) << "Continue play of MusicPlayer ...";

    switch (m_playerType)
    {
    case 2:
        spotifyPlayer->play();
        emit startedPlaying();
        break;

    default:
        mediaPlayer->play();
        break;
    }
}

void MusicPlayer::startPlaying()
{
    qCDebug(gmAudioMusic) << "startPlaying()";

    if (currentElement && (m_playlist.length() > 0))
    {
        applyShuffleMode();

        if (currentElement->mode() == 1)
        {
            next();
            return;
        }

        loadMedia(m_playlist.first());
    }
}

void MusicPlayer::loadSongNames(bool initial, bool reloadYt)
{
    qCDebug(gmAudioMusic()) << "loadSongNames()" << initial << reloadYt;
    m_songNames.clear();

    for (auto *s : m_playlist)
    {
        switch (s->source())
        {
        case 0: // Local files
            m_songNames.append(s->url().right(s->url().length() - s->url().lastIndexOf("/") - 1));
            break;

        case 2: // Spotify
            m_songNames.append(s->title().isEmpty() ? s->url() : s->title());

            if (initial) {
                spotifyPlayer->getPlaylistTracks(s->url());
                m_waitingForUrls++;
            }
            break;

        case 3: // Youtube
            m_songNames.append(s->title().isEmpty() ? s->url() : s->title());

            if ((initial || reloadYt) && s->title().isEmpty())
            {
                auto *video = videoClient->getVideo(s->url());
                connect(video, &Video::ready, this, &MusicPlayer::onVideoMetadataReceived);
            }
            break;

        default:
            m_songNames.append(s->url());
            break;
        }
    }

    emit songNamesChanged();

    if (initial && !m_waitingForUrls) startPlaying();
}

void MusicPlayer::applyShuffleMode(bool keepIndex, const QString& url)
{
    qCDebug(gmAudioMusic) << "Applying shuffle mode" << currentElement->mode();

    // Playback mode
    if (currentElement->mode() == 0)
    {
        QString currentUrl = url.isEmpty() ? m_playlist[m_playlistIndex]->url() : url;

        qCDebug(gmAudioMusic) << "Unshuffled playlist:";

        for (auto *e : m_playlist) qCDebug(gmAudioMusic) << e->url();

        #if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))

        QList<AudioFile *> temp;

        while (!m_playlist.isEmpty())
        {
            temp.append(m_playlist.takeAt(QRandomGenerator::global()->bounded(m_playlist.size())));
        }
        m_playlist = temp;

        #else // if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
        std::random_device rd;
        std::mt19937 gen(rd());
        QList<AudioFile*> temp;

        while (!m_playlist.isEmpty())
        {
            std::uniform_int_distribution<> dis(0, m_playlist.length() - 1);
            temp.append(m_playlist.takeAt(dis(gen)));
        }
        m_playlist = temp;
        #endif // if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))

        qCDebug(gmAudioMusic) << "Shuffled playlist:";

        for (auto *e : m_playlist) qCDebug(gmAudioMusic) << e->url();

        if (keepIndex)
        {
            int index = 0;

            for (auto *e : m_playlist)
            {
                if (e->url().contains(currentUrl))
                {
                    #if (QT_VERSION >= QT_VERSION_CHECK(5, 13, 0))
                    m_playlist.swapItemsAt(m_playlistIndex, index);
                    #else // if (QT_VERSION >= QT_VERSION_CHECK(5, 13, 0))
                    m_playlist.swap(m_playlistIndex, index);
                    #endif // if (QT_VERSION >= QT_VERSION_CHECK(5, 13, 0))
                    loadSongNames();
                    return;
                }
                index++;
            }
        }

        loadSongNames();
    }
}

void MusicPlayer::loadMedia(AudioFile *file)
{
    qCInfo(gmAudioMusic) << "Loading media (" << file->url() << ") ...";

    mediaPlayer->stop();
    spotifyPlayer->stop();
    m_playerType = file->source();
    emit currentIndexChanged();
    emit clearMetaData();

    auto useDiscord = Discord::getInstance()->enabled();

    switch (m_playerType)
    {
    case 0:
    {
        m_fileRequestId = FileManager::getUniqueRequestId();
        m_fileName = file->url();
        FileManager::getInstance()->getFile(m_fileRequestId, SettingsManager::getPath("music") + file->url());
        break;
    }

    case 1:
    {
        mediaPlayer->setMedia(QUrl(file->url()));
        mediaPlayer->play();
        mediaPlayer->setMuted(useDiscord);
        if (useDiscord) discordPlayer->playMusic(file->url());
        break;
    }

    case 2:
    {
        spotifyPlayer->play(file->url(), 0, true);
        emit startedPlaying();
        break;
    }

    case 3:
    {
        qCDebug(gmAudioMusic) << "Media is a youtube video ...";
        m_streamManifest = videoClient->streams()->getManifest(file->url());
        connect(m_streamManifest, &Streams::StreamManifest::ready, this, &MusicPlayer::onStreamManifestReceived);

        auto *video = videoClient->getVideo(file->url());
        connect(video, &Video::ready, this, &MusicPlayer::onVideoMetadataReceived);

        mediaPlayer->setMuted(useDiscord);
        if (useDiscord) discordPlayer->playMusic(file->url());
        break;
    }

    default:
        next();
        break;
    }
}

/**
 * @brief Pause the music playback
 */
void MusicPlayer::pause()
{
    qCDebug(gmAudioMusic) << "Pausing MusicPlayer ...";

    switch (m_playerType)
    {
    case 2:
        spotifyPlayer->pause();
        break;

    default:
        mediaPlayer->pause();
        break;
    }
}

/**
 * @brief Stop playing music
 */
void MusicPlayer::stop()
{
    qCDebug(gmAudioMusic) << "Stopping MusicPlayer ...";

    spotifyPlayer->stop();
    mediaPlayer->stop();
    m_songNames.clear();
    emit songNamesChanged();
}

void MusicPlayer::next()
{
    emit clearMetaData();

    if (!currentElement || (m_playlist.length() < 1)) return;

    // Complete random
    if (currentElement->mode() == 1)
    {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
        m_playlistIndex = QRandomGenerator::system()->bounded(m_playlist.length());
#else // if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, m_playlist.length() - 1);
        m_playlistIndex = dis(gen);
#endif // if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
        loadMedia(m_playlist[m_playlistIndex]);
    }
    else
    {
        // choose next in line (mode 0, 2, 3)
        if (m_playlistIndex < m_playlist.length() - 1)
        {
            loadMedia(m_playlist[++m_playlistIndex]);
        }

        // loop around (Mode 0 or 2)
        else if (currentElement->mode() != 3)
        {
            m_playlistIndex = 0;
            loadMedia(m_playlist[0]);
        }
    }
}

void MusicPlayer::again()
{
    switch (m_playerType)
    {
    case 2:
        spotifyPlayer->again();
        break;

    default:
        mediaPlayer->setPosition(0);
        break;
    }
}

void MusicPlayer::setIndex(int index)
{
    m_playlistIndex = index;
    loadMedia(m_playlist[index]);
}

void MusicPlayer::setLogarithmicVolume(int volume)
{
    mediaPlayer->setVolume(volume);
}

void MusicPlayer::setLinearVolume(int volume)
{
    spotifyPlayer->setLinearVolume(volume);
}

void MusicPlayer::onMediaPlayerStateChanged()
{
    qCDebug(gmAudioMusic) << "Media player state changed:" << mediaPlayer->state();

    if (mediaPlayer->state() == QMediaPlayer::PlayingState) emit startedPlaying();
}

void MusicPlayer::onMediaPlayerBufferStatusChanged()
{
    qCDebug(gmAudioMusic) << "Buffer status:" << mediaPlayer->bufferStatus();
}

void MusicPlayer::onMediaPlayerMediaStatusChanged()
{
    if (mediaPlayer->mediaStatus() == QMediaPlayer::EndOfMedia)
    {
        qCDebug(gmAudioMusic()) << "End of media was reached, starting next song ...";
        next();
    }
    else if (mediaPlayer->mediaStatus() == QMediaPlayer::BufferedMedia)
    {
        emit metaDataChanged(mediaPlayer);
    }
}

void MusicPlayer::onMediaPlayerError(QMediaPlayer::Error error)
{
    qCWarning(gmAudioMusic()) << error;

    if (error == QMediaPlayer::FormatError)
    {
        next();
    }
}

void MusicPlayer::onMediaPlayerMediaChanged()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    qCDebug(gmAudioMusic) << "Media changed:" << mediaPlayer->media().request().url();
#else // if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    qCDebug(gmAudioMusic) << "Media changed:" << mediaPlayer->media().canonicalUrl();
#endif // if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
}

void MusicPlayer::onFileReceived(int id, const QByteArray& data)
{
    if (m_fileRequestId != id) return;

    qCDebug(gmAudioMusic()) << "Received file ...";

    // On Windows a there seems to be a weird issue with
    // a strange clicking noise at the beginning of files.
    // Muting the mediaPlayer and unmuting about 100ms into
    // the song seems to be a workaround.
    auto clickingWorkaround = false;

    #ifdef Q_OS_WIN
    clickingWorkaround = true;
    #endif

    if (clickingWorkaround) mediaPlayer->setMuted(true);
    mediaPlayer->stop();

    if (data.isEmpty())
    {
        qCWarning(gmAudioMusic()) << "File is empty, skipping ...";
        next();
        return;
    }

    #ifdef Q_OS_WIN
    QFile file(m_tempDir.path() + "/" +  FileUtils::fileName(m_fileName));

    if (!file.open(QIODevice::WriteOnly))
    {
        if (file.error() == QFileDevice::OpenError)
        {
            file.setFileName(FileUtils::incrementFileName(file.fileName()));
            if (!file.open(QIODevice::WriteOnly))
            {
                qCWarning(gmAudioSounds()) << "Error: Could not open temporary file even after incrementing the filename" << file.fileName() << file.errorString();
                return;
            }
        }
        else
        {
            qCWarning(gmAudioSounds()) << "Error: Could not open temporary file:" << file.fileName() << file.errorString();
            return;
        }
    }

    qCDebug(gmAudioMusic()) << file.fileName();
    file.write(data);
    file.close();

    mediaPlayer->setMedia(QMediaContent(QUrl::fromLocalFile(file.fileName())), nullptr);
    #else
    m_mediaBuffer.close();
    m_mediaBuffer.setData(data);
    m_mediaBuffer.open(QIODevice::ReadOnly);
    mediaPlayer->setMedia(QMediaContent(), &m_mediaBuffer);
    #endif

    auto useDiscord = Discord::getInstance()->enabled();

    mediaPlayer->setMuted(useDiscord);
    if (useDiscord) discordPlayer->playMusic(data);

    mediaPlayer->play();

    if (clickingWorkaround && !useDiscord) QTimer::singleShot(100, [ = ]() { mediaPlayer->setMuted(false); });

    qCDebug(gmAudioMusic()) << "Sending file data to metadatareader ...";

    emit metaDataChanged(data);
}

void MusicPlayer::onSpotifySongEnded()
{
    qCDebug(gmAudioMusic()) << "Spotify song ended, starting next song ...";
    next();
}

void MusicPlayer::onSpotifyReceivedPlaylistTracks(QList<SpotifyTrack>tracks, const QString& playlistId)
{
    qCDebug(gmAudioMusic) << "Received" << tracks.length() << "spotify playlist tracks ...";

    if (tracks.length() < 1) return;

    auto wasWaiting = m_waitingForUrls > 0;
    qCDebug(gmAudioMusic) << "Are we waiting for track names?" << wasWaiting;

    for (int i = 0; i < m_playlist.count(); i++)
    {
        if ((m_playlist[i]->source() == 2) && m_playlist[i]->url().contains(playlistId))
        {
            if (m_playlistIndex > i) m_playlistIndex += tracks.count();

            m_playlist.removeAt(i);

            for (int j = 0; j < tracks.count(); j++)
            {
                m_playlist.insert(i + j, new AudioFile(tracks[j].uri, 2, tracks[j].title, currentElement));
            }

            emit songNamesChanged();
            emit currentIndexChanged();

            if (wasWaiting)
            {
                m_waitingForUrls--;

                if (!m_waitingForUrls) startPlaying();
            }

            return;
        }
    }
}

void MusicPlayer::onVideoMetadataReceived()
{
    auto *video = qobject_cast<YouTube::Videos::Video*>(sender());
    if (!video) return;

    for (int i = 0; i < m_playlist.count(); i++)
    {
        if ((m_playlist[i]->source() == 3) && (VideoId::normalize(m_playlist[i]->url()) == VideoId::normalize(video->id())))
        {
            m_playlist[i]->setTitle(video->title());

            if (i == m_playlistIndex)
            {
                emit metaDataChanged("Title", video->title());
                emit metaDataChanged("Artist", video->author());
            }

            loadSongNames();
            video->deleteLater();
            return;
        }
    }

    video->deleteLater();
}

void MusicPlayer::onStreamManifestReceived()
{
    auto *manifest = qobject_cast<Streams::StreamManifest*>(sender());
    if (!manifest || manifest != m_streamManifest) return;

    qCDebug(gmAudioMusic) << "Received youtube media streams:" << manifest->audio().length();

    // Find best audio stream that qmediaplayer supports
    auto audioStreams = manifest->audio();
    bool foundGoodStream = false;

    do
    {
        auto *stream = Streams::AudioOnlyStreamInfo::withHighestBitrate(audioStreams);

        if (stream && QMediaPlayer::hasSupport("audio/" + stream->audioCodec(),
            { stream->audioCodec() }, QMediaPlayer::StreamPlayback) > 0)
        {
            mediaPlayer->setMedia(QUrl(stream->url()));
            mediaPlayer->play();
            loadSongNames(false, true);
            foundGoodStream = true;
            break;
        }
        else
        {
            if (stream) qCDebug(gmAudioMusic()) << "Audio codec" << stream->audioCodec() << "is not supported on this device.";

            audioStreams.removeOne(stream);
        }
    }
    while (!audioStreams.isEmpty());

    if (!foundGoodStream)
    {
        qCWarning(gmAudioMusic()) << "Error: Could not find an audio stream that is supported on your device.";
    }

    manifest->deleteLater();
}
