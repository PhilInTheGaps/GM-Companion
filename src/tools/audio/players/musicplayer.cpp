#include "musicplayer.h"
#include <QDebug>
#include <algorithm>
#include <random>
#include <cstdlib>
#include "youtubeutils.h"
#include "internal/heuristics.h"
#include <QTimer>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
# include <QRandomGenerator>
#endif // if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))

MusicPlayer::MusicPlayer(FileManager *fManager, SpotifyPlayer *spotify)
{
    qDebug() << "Loading MusicPlayer ...";

    fileManager         = fManager;
    this->spotifyPlayer = spotify;

    mediaPlayer = new QMediaPlayer;
    mediaPlayer->setObjectName(tr("Music"));

    connect(mediaPlayer, &QMediaPlayer::stateChanged,                              this, &MusicPlayer::onMediaPlayerStateChanged);
    connect(mediaPlayer, &QMediaPlayer::bufferStatusChanged,                       this, &MusicPlayer::onMediaPlayerBufferStatusChanged);
    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged,                        this, &MusicPlayer::onMediaPlayerMediaStatusChanged);
    connect(mediaPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &MusicPlayer::onMediaPlayerError);
    connect(mediaPlayer, &QMediaPlayer::mediaChanged,                              this, &MusicPlayer::onMediaPlayerMediaChanged);

    connect(spotify,     &SpotifyPlayer::songEnded,                                this, &MusicPlayer::onSpotifySongEnded);
    connect(spotify,     &SpotifyPlayer::receivedPlaylistTracks,                   this, &MusicPlayer::onSpotifyReceivedPlaylistTracks);

    connect(&youtube,    &YouTube::receivedVideoMediaStreamInfos,                  this, &MusicPlayer::onYtReceivedVideoMediaStreamInfos);
    connect(&youtube,    &YouTube::receivedVideo,                                  this, &MusicPlayer::onYtReceivedVideo);

    connect(mediaPlayer, QOverload<>::of(&QMediaObject::metaDataChanged),          this, &MusicPlayer::onMetaDataChanged);
}

MusicPlayer::~MusicPlayer()
{
    mediaPlayer->deleteLater();
}

/**
 * @brief Play music list
 * @param name Name of the element
 */
void MusicPlayer::play(MusicElement *element)
{
    if (!element) return;

    qDebug() << "Playing music list:" << element->name();

    mediaPlayer->stop();
    mediaPlayer->setObjectName(tr("Music") + ": " + element->name());
    m_playlist.clear();
    m_playlistIndex = 0;

    currentElement   = element;
    m_waitingForUrls = true;

    m_playlist = element->files();

    applyShuffleMode();
    loadSongNames(true);

    emit songNamesChanged();

    if (m_playlist.length() > 0)
    {
        if (element->mode() == 1)
        {
            next();
            return;
        }

        loadMedia(m_playlist.first());
    }
}

/**
 * @brief Start playing music
 */
void MusicPlayer::play()
{
    qDebug() << "Continue play of MusicPlayer ...";

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

void MusicPlayer::loadSongNames(bool initial, bool reloadYt)
{
    m_songNames.clear();

    for (auto s : m_playlist)
    {
        switch (s.source())
        {
        case 0:
            m_songNames.append(s.url().right(s.url().length() - s.url().lastIndexOf("/") - 1));
            break;

        case 2:
            m_songNames.append(s.title().isEmpty() ? s.url() : s.title());

            if (initial) spotifyPlayer->getPlaylistTracks(s.url());
            break;

        case 3:
            m_songNames.append(s.title().isEmpty() ? s.url() : s.title());

            if ((initial || reloadYt) && s.title().isEmpty()) youtube.getVideo(YouTubeUtils::parseVideoId(s.url()));
            break;

        default:
            m_songNames.append(s.url());
            break;
        }
    }
}

void MusicPlayer::applyShuffleMode(bool keepIndex, QString url)
{
    // Playback mode
    if (currentElement->mode() == 0)
    {
        QString currentUrl = url.isEmpty() ? m_playlist[m_playlistIndex].url() : url;

        for (auto e : m_playlist) qDebug() << e.url();

#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))

        QList<AudioFile> temp;

        while (!m_playlist.isEmpty())
        {
            temp.append(m_playlist.takeAt(QRandomGenerator::global()->bounded(m_playlist.size())));
        }
        m_playlist = temp;

#else // if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
        std::random_device rd;
        std::mt19937 gen(rd());
        QList<AudioFile> temp;

        while (!m_playlist.isEmpty())
        {
            std::uniform_int_distribution<> dis(0, m_playlist.length() - 1);
            temp.append(m_playlist.takeAt(dis(gen)));
        }
        m_playlist = temp;
#endif // if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))

        if (keepIndex)
        {
            int index = 0;

            for (auto e : m_playlist)
            {
                if (e.url().contains(currentUrl))
                {
                    //                    m_playlistIndex = index;
                    m_playlist.swapItemsAt(m_playlistIndex, index);
                    return;
                }
                index++;
            }
        }
    }
}

void MusicPlayer::loadMedia(AudioFile file)
{
    qDebug() << "MusicPlayer: Loading media (" << file.url() << ") ...";

    mediaPlayer->stop();
    spotifyPlayer->stop();
    m_playerType = file.source();
    emit currentIndexChanged();

    switch (m_playerType)
    {
    case 0:
        mediaPlayer->setMedia(QUrl::fromLocalFile(sManager.getSetting(Setting::musicPath) + file.url()));
        mediaPlayer->play();
        break;

    case 1:
        mediaPlayer->setMedia(QUrl(file.url()));
        mediaPlayer->play();
        break;

    case 2:
        spotifyPlayer->play(file.url(), 0, true);
        emit startedPlaying();
        break;

    case 3:
        m_youtubeRequestId = youtube.getVideoAudioStreamInfo(YouTubeUtils::parseVideoId(file.url()));
        break;

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
    qDebug() << "Pausing MusicPlayer ...";

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
    qDebug() << "Stopping MusicPlayer ...";

    spotifyPlayer->stop();
    mediaPlayer->stop();
    m_songNames.clear();
    emit songNamesChanged();
}

void MusicPlayer::next()
{
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

void MusicPlayer::setVolume(float volume)
{
    mediaPlayer->setVolume(volume);
}

void MusicPlayer::setVolume(int volume)
{
    spotifyPlayer->setVolume(volume);
}

/**
 * @brief Tell MetaDataReader that new MetaData is available
 */
void MusicPlayer::onMetaDataChanged()
{
    if ((mediaPlayer->bufferStatus() == 100) || (mediaPlayer->mediaStatus() == QMediaPlayer::BufferedMedia))
    {
        qDebug() << "MusicPlayer: MetaData changed!";

        emit metaDataChanged(mediaPlayer, currentElement ? currentElement->icon()->background() : "");
    }
}

void MusicPlayer::onMediaPlayerStateChanged()
{
    if (mediaPlayer->state() == QMediaPlayer::PlayingState) emit startedPlaying();
}

void MusicPlayer::onMediaPlayerBufferStatusChanged()
{
    qDebug() << "BUFFER STATUS:" << mediaPlayer->bufferStatus();
}

void MusicPlayer::onMediaPlayerMediaStatusChanged()
{
    if (mediaPlayer->mediaStatus() == QMediaPlayer::MediaStatus::EndOfMedia)
    {
        next();
    }
}

void MusicPlayer::onMediaPlayerError(QMediaPlayer::Error error)
{
    qWarning() << error;

    if ((error == QMediaPlayer::FormatError) ||
        (error == QMediaPlayer::FormatError))
    {
        next();
    }
}

void MusicPlayer::onMediaPlayerMediaChanged()
{
    qDebug() << "MEDIA CHANGED:" << mediaPlayer->media().canonicalUrl();
}

void MusicPlayer::onSpotifySongEnded()
{
    next();
}

void MusicPlayer::onSpotifyReceivedPlaylistTracks(QList<SpotifyTrack>tracks, QString playlistId)
{
    for (int i = 0; i < m_playlist.count(); i++)
    {
        if ((m_playlist[i].source() == 2) && m_playlist[i].url().contains(playlistId))
        {
            if (m_playlistIndex > i) m_playlistIndex += tracks.count();

            m_playlist.removeAt(i);
            SpotifyTrack firstPlaylistTrack = tracks.first();

            for (int j = 0; j < tracks.count(); j++)
            {
                m_playlist.insert(i + j, AudioFile(tracks[j].id, 2, tracks[j].title));
            }

            applyShuffleMode(true, m_playlistIndex == i ? firstPlaylistTrack.id : "");
            loadSongNames();

            emit songNamesChanged();
            emit currentIndexChanged();
            return;
        }
    }
}

void MusicPlayer::onYtReceivedVideoMediaStreamInfos(MediaStreamInfoSet *infos, int reqId)
{
    if (reqId != m_youtubeRequestId) return;

    QUrl   tempUrl;
    qint64 tempBitrate = 0;

    for (auto info : infos->audio())
    {
        auto encoding = Heuristics::audioEncodingToString(info->audioEncoding());
        qDebug() << info->audioEncoding() << info->bitrate()
                 << mediaPlayer->hasSupport("audio/" + encoding, { encoding });

        if ((mediaPlayer->hasSupport("audio/" + encoding, { encoding }) > 1) && (info->bitrate() > tempBitrate))
        {
            tempUrl     = info->url();
            tempBitrate = info->bitrate();
        }
    }

    mediaPlayer->setMedia(tempUrl);
    mediaPlayer->play();
    loadSongNames(false, true);

    delete infos;
}

void MusicPlayer::onYtReceivedVideo(Video video, int reqId)
{
    for (int i = 0; i < m_playlist.count(); i++)
    {
        if ((m_playlist[i].source() == 3) && (YouTubeUtils::parseVideoId(m_playlist[i].url()) == video.id()))
        {
            m_playlist[i].setTitle(video.title());
            loadSongNames();
            emit songNamesChanged();
            return;
        }
    }
}
