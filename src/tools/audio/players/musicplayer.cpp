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

MusicPlayer::MusicPlayer(FileManager *fManager, Spotify *spotify)
{
    qDebug() << "Loading MusicPlayer ...";

    fileManager   = fManager;
    this->spotify = spotify;

    player = new QMediaPlayer;
    player->setObjectName(tr("Music"));

    connect(player,   &QMediaPlayer::stateChanged,                              this, &MusicPlayer::onMediaPlayerStateChanged);
    connect(player,   &QMediaPlayer::bufferStatusChanged,                       this, &MusicPlayer::onMediaPlayerBufferStatusChanged);
    connect(player,   &QMediaPlayer::mediaStatusChanged,                        this, &MusicPlayer::onMediaPlayerMediaStatusChanged);
    connect(player,   QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &MusicPlayer::onMediaPlayerError);
    connect(player,   &QMediaPlayer::mediaChanged,                              this, &MusicPlayer::onMediaPlayerMediaChanged);

    connect(spotify,  &Spotify::songEnded,                                      this, &MusicPlayer::onSpotifySongEnded);
    connect(spotify,  &Spotify::receivedPlaylistTracks,                         this, &MusicPlayer::onSpotifyReceivedPlaylistTracks);

    connect(&youtube, &YouTube::receivedVideoMediaStreamInfos,                  this, &MusicPlayer::onYtReceivedVideoMediaStreamInfos);
    connect(&youtube, &YouTube::receivedVideo,                                  this, &MusicPlayer::onYtReceivedVideo);

    connect(player,   QOverload<>::of(&QMediaObject::metaDataChanged),          this, &MusicPlayer::onMetaDataChanged);
}

MusicPlayer::~MusicPlayer()
{
    player->deleteLater();
}

/**
 * @brief Play music list
 * @param name Name of the element
 */
void MusicPlayer::play(MusicElement *element)
{
    if (!element) return;

    qDebug() << "Playing music list:" << element->name();

    player->stop();
    player->setObjectName(tr("Music") + ": " + element->name());
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

            if (initial) spotify->getPlaylistTracks(s.url());
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
            qDebug() << "---";

            for (auto e : m_playlist) qDebug() << e.url();
            qDebug() << "Current:" << m_playlistIndex << currentUrl;

            int index = 0;

            for (auto e : m_playlist)
            {
                if (e.url().contains(currentUrl))
                {
                    qDebug() << "New:" << index << e.url();
                    m_playlistIndex = index;
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

    player->stop();
    spotify->stop();
    m_playerType = file.source();

    switch (m_playerType)
    {
    case 0:
        player->setMedia(QUrl::fromLocalFile(sManager.getSetting(Setting::musicPath) + file.url()));
        player->play();
        break;

    case 1:
        player->setMedia(QUrl(file.url()));
        player->play();
        break;

    case 2:
        spotify->play(file.url(), 0, true);
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
 * @brief Start playing music
 */
void MusicPlayer::play()
{
    qDebug() << "Continue play of MusicPlayer ...";

    switch (m_playerType)
    {
    case 2:
        spotify->play();
        emit startedPlaying();
        break;

    default:
        player->play();
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
        spotify->pause();
        break;

    default:
        player->pause();
        break;
    }
}

/**
 * @brief Stop playing music
 */
void MusicPlayer::stop()
{
    qDebug() << "Stopping MusicPlayer ...";

    spotify->stop();
    player->stop();
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
        spotify->again();
        break;

    default:
        player->setPosition(0);
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
    player->setVolume(volume);
}

void MusicPlayer::setVolume(int volume)
{
    spotify->setVolume(volume);
}

/**
 * @brief Tell MetaDataReader that new MetaData is available
 */
void MusicPlayer::onMetaDataChanged()
{
    if ((player->bufferStatus() == 100) || (player->mediaStatus() == QMediaPlayer::BufferedMedia))
    {
        qDebug() << "MusicPlayer: MetaData changed!";

        emit metaDataChanged(player, currentElement ? currentElement->icon() : "");
    }
}

void MusicPlayer::onMediaPlayerStateChanged()
{
    if (player->state() == QMediaPlayer::PlayingState) emit startedPlaying();
}

void MusicPlayer::onMediaPlayerBufferStatusChanged()
{
    qDebug() << "BUFFER STATUS:" << player->bufferStatus();
}

void MusicPlayer::onMediaPlayerMediaStatusChanged()
{
    if (player->mediaStatus() == QMediaPlayer::MediaStatus::EndOfMedia)
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
    qDebug() << "MEDIA CHANGED:" << player->media().canonicalUrl();
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
                 << player->hasSupport("audio/" + encoding, { encoding });

        if ((player->hasSupport("audio/" + encoding, { encoding }) > 1) && (info->bitrate() > tempBitrate))
        {
            tempUrl     = info->url();
            tempBitrate = info->bitrate();
        }
    }

    player->setMedia(tempUrl);
    player->play();
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
