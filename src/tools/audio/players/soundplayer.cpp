#include "soundplayer.h"
#include "logging.h"
#include "filesystem/filemanager.h"
#include "settings/settingsmanager.h"

#include "youtubeutils.h"
#include <internal/heuristics.h>
#include <algorithm>
#include <random>
#include <cstdlib>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
# include <QRandomGenerator>
#endif // if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))

/**
 * @brief Play a sound element
 * @param element SoundElement to be played
 */
void SoundPlayerController::play(AudioElement *element)
{
    if (!element) return;

    qCDebug(gmAudioSounds()) << "Playing sound list:" << element->name() << "...";

    if (!isSoundPlaying(element))
    {
        auto *player = new SoundPlayer(element, m_volume, this);

        connect(player, &SoundPlayer::playerStopped,         this,   &SoundPlayerController::onPlayerStopped);
        connect(this,   &SoundPlayerController::setVolume,   player, &SoundPlayer::setLogarithmicVolume);
        connect(this,   &SoundPlayerController::stopAll,     player, &SoundPlayer::stop);
        connect(this,   &SoundPlayerController::stopElement, player, &SoundPlayer::stopElement);

        m_players.append(player);
        player->play();

        emit soundsChanged(elements());
    }
}

/**
 * @brief Check if a sound element is currently playing
 * @param element SoundElement to be checked
 * @return True if element is playing
 */
auto SoundPlayerController::isSoundPlaying(AudioElement *element) -> bool
{
    qCDebug(gmAudioSounds()) << "Checking if sound" << element->name() << "is playing ...";

    for (auto *player : m_players)
    {
        if (player && (player->element() == element))
        {
            return true;
        }
    }

    return false;
}

/**
 * @brief Set the volume for all active sound elements
 * @param volume Volume value
 */
void SoundPlayerController::setLogarithmicVolume(int volume)
{
    m_volume = volume;
    emit setVolume(volume);
}

/**
 * @brief Get all active sound elements
 */
auto SoundPlayerController::elements() const -> QList<AudioElement *>
{
    QList<AudioElement *> elements;

    for (auto *player : m_players)
    {
        if (player) elements.append(player->element());
    }

    return elements;
}

/**
 * @brief A sound player stopped. Remove it from list and delete it.
 */
void SoundPlayerController::onPlayerStopped(SoundPlayer *player)
{
    m_players.removeOne(player);
    delete player;
    emit soundsChanged(elements());
}

SoundPlayer::SoundPlayer(AudioElement *element, int volume, QObject *parent) :
    AudioPlayer(parent), m_element(element)
{
    if (!element)
    {
        qCCritical(gmAudioSounds()) << "Error: could not initialize soundplayer, element is null";
        return;
    }

    m_mediaPlayer = new QMediaPlayer;
    m_mediaPlayer->setObjectName(element->name());
    m_mediaPlayer->setVolume(volume);

    connect(m_mediaPlayer,              &QMediaPlayer::mediaStatusChanged,                        this, &SoundPlayer::onMediaStatusChanged);
    connect(m_mediaPlayer,              QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &SoundPlayer::onMediaPlayerError);
    connect(&youtube,                   &YouTube::receivedVideoMediaStreamInfos,                  this, &SoundPlayer::onReceivedAudioStreamInfo);
    connect(FileManager::getInstance(), &FileManager::receivedFile,                               this, &SoundPlayer::onFileReceived);

    m_playlist = element->files();
    applyShuffleMode();
}

SoundPlayer::~SoundPlayer()
{
    m_mediaPlayer->stop();
    m_mediaPlayer->deleteLater();
}

void SoundPlayer::loadMedia(AudioFile *file)
{
    qCDebug(gmAudioSounds()) << "Loading media" << file->url();

    switch (file->source())
    {
    case 0:
        m_fileRequestId = FileManager::getUniqueRequestId();
        FileManager::getInstance()->getFile(m_fileRequestId, SettingsManager::getPath("sounds") + file->url());
        break;

    case 1:
        m_mediaPlayer->setMedia(QUrl(file->url()));
        m_mediaPlayer->play();
        break;

    case 3:
        m_youtubeRequestId = youtube.getVideoAudioStreamInfo(YouTubeUtils::parseVideoId(file->url()));
        break;

    default:
        qCWarning(gmAudioSounds()) << "Media type" << file->source() << "is currently not supported.";
    }
}

void SoundPlayer::play()
{
    if (m_element->mode() == 1)
    {
        next();
    }
    else
    {
        loadMedia(m_playlist.first());
    }
}

void SoundPlayer::stop()
{
    m_mediaPlayer->stop();
    emit playerStopped(this);
}

void SoundPlayer::stopElement(const QString& element)
{
    if (m_element && (m_element->name() == element))
    {
        qCDebug(gmAudioSounds()) << "Stopping" << element;
        m_mediaPlayer->stop();
        emit playerStopped(this);
    }
}

void SoundPlayer::next()
{
    if (!m_element || (m_playlist.length() < 1))
    {
        emit playerStopped(this);
        return;
    }

    // Complete random
    if (m_element->mode() == 1)
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
        return;
    }

    // choose next in line (mode 0, 2, 3)
    if (m_playlistIndex < m_playlist.length() - 1)
    {
        loadMedia(m_playlist[++m_playlistIndex]);
    }

    // loop around (Mode 0 or 2)
    else if (m_element->mode() != 3)
    {
        m_playlistIndex = 0;
        loadMedia(m_playlist[0]);
    }

    // reached end of playlist, stop
    else
    {
        emit playerStopped(this);
    }
}

void SoundPlayer::applyShuffleMode()
{
    if (m_element->mode() != 0) return;

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
    QList<AudioFile> temp;

    while (!m_playlist.isEmpty())
    {
        std::uniform_int_distribution<> dis(0, m_playlist.length() - 1);
        temp.append(m_playlist.takeAt(dis(gen)));
    }
    m_playlist = temp;
#endif // if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
}

void SoundPlayer::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia)
    {
        next();
    }
}

void SoundPlayer::onMediaPlayerError(QMediaPlayer::Error error)
{
    qCWarning(gmAudioSounds()) << error << m_mediaPlayer->errorString();

    if (error == QMediaPlayer::FormatError)
    {
        next();
    }
}

void SoundPlayer::onReceivedAudioStreamInfo(MediaStreamInfoSet *infos, int requestId)
{
    if (m_youtubeRequestId != requestId) return;

    QUrl   tempUrl;
    qint64 tempBitrate = 0;

    for (auto *info : infos->audio())
    {
        auto encoding = Heuristics::audioEncodingToString(info->audioEncoding());
        qCDebug(gmAudioSounds()) << info->audioEncoding() << info->bitrate()
                                 << QMediaPlayer::hasSupport("audio/" + encoding, { encoding });

        if ((QMediaPlayer::hasSupport("audio/" + encoding, { encoding }) > 1) && (info->bitrate() > tempBitrate))
        {
            tempUrl     = info->url();
            tempBitrate = info->bitrate();
        }
    }

    m_mediaPlayer->setMedia(tempUrl);

    if (m_mediaPlayer->state() != QMediaPlayer::PlayingState)
    {
        m_mediaPlayer->play();
    }

    delete infos;
}

void SoundPlayer::onFileReceived(int requestId, const QByteArray& data)
{
    if (m_fileRequestId != requestId) return;

    m_mediaPlayer->stop();

    if (data.isEmpty())
    {
        next();
        return;
    }

    delete m_mediaBuffer;

    m_mediaData   = data;
    m_mediaBuffer = new QBuffer(&m_mediaData);
    m_mediaBuffer->open(QIODevice::ReadOnly);
    m_mediaBuffer->seek(0);

    m_mediaPlayer->setMedia(QMediaContent(), m_mediaBuffer);
    m_mediaPlayer->play();
}
