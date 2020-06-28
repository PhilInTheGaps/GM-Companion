#include "soundplayer.h"
#include "logging.h"
#include "filesystem/filemanager.h"
#include "settings/settingsmanager.h"
#include "utils/fileutils.h"

#include <algorithm>
#include <random>
#include <cstdlib>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
# include <QRandomGenerator>
#endif // if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))

using namespace YouTube::Videos;

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
        auto *player = new SoundPlayer(element, m_volume, m_networkManager, m_discordPlayer, this);

        connect(player, &SoundPlayer::playerStopped,         this,   &SoundPlayerController::onPlayerStopped);
        connect(this,   &SoundPlayerController::setVolume,   player, &SoundPlayer::setLogarithmicVolume);
        connect(this,   &SoundPlayerController::stopAll,     player, &SoundPlayer::stop);
        connect(this,   &SoundPlayerController::stopElement, player, &SoundPlayer::stopElement);

        m_players.append(player);
        player->play();

        emit soundsChanged(elements());
    }
}

void SoundPlayerController::stop(QString element)
{
    if (Discord::getInstance()->enabled())
    {
        for (const auto *player : m_players)
        {
            if (player->element() && player->element()->name() == element)
            {
                m_discordPlayer->stopSound(player->fileName());
                break;
            }
        }
    }

    emit stopElement(element);
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

SoundPlayer::SoundPlayer(AudioElement *element, int volume, QNetworkAccessManager *networkManager, DiscordPlayer *discordPlayer, QObject *parent) :
    AudioPlayer(parent), m_element(element), m_discordPlayer(discordPlayer)
{
    if (!element)
    {
        qCWarning(gmAudioSounds()) << "Error: could not initialize soundplayer, element is null";
        return;
    }

    m_mediaPlayer = new QMediaPlayer;
    m_mediaPlayer->setObjectName(element->name());
    m_mediaPlayer->setVolume(volume);
    m_videoClient = new YouTube::Videos::VideoClient(networkManager, this);

    connect(m_mediaPlayer,              &QMediaPlayer::mediaStatusChanged,                        this, &SoundPlayer::onMediaStatusChanged);
    connect(m_mediaPlayer,              QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &SoundPlayer::onMediaPlayerError);
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

    auto useDiscord = Discord::getInstance()->enabled();

    switch (file->source())
    {
    case 0:
    {
        m_fileRequestId = FileManager::getUniqueRequestId();
        m_fileName = file->url();
        FileManager::getInstance()->getFile(m_fileRequestId, SettingsManager::getPath("sounds") + file->url());
        break;
    }

    case 1:
    {
        m_mediaPlayer->setMedia(QUrl(file->url()));
        m_mediaPlayer->play();
        m_mediaPlayer->setMuted(useDiscord);
        m_fileName = file->url();
        if (useDiscord) m_discordPlayer->playSound(file->url());
        break;
    }

    case 3:
    {
        m_streamManifest = m_videoClient->streams()->getManifest(file->url());
        connect(m_streamManifest, &Streams::StreamManifest::ready, this, &SoundPlayer::onStreamManifestReceived);
        m_mediaPlayer->setMuted(useDiscord);
        m_fileName = file->url();
        if (useDiscord) m_discordPlayer->playSound(file->url());
        break;
    }

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
    QList<AudioFile*> temp;

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
    qCDebug(gmAudioSounds()) << "Status changed:" << status;

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

void SoundPlayer::onFileReceived(int requestId, const QByteArray& data)
{
    if (m_fileRequestId != requestId) return;

    m_mediaPlayer->stop();

    if (data.isEmpty())
    {
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

    qCDebug(gmAudioSounds()) << file.fileName();
    file.write(data);
    file.close();

    m_mediaPlayer->setMedia(QMediaContent(QUrl::fromLocalFile(file.fileName())), nullptr);
    #else
    m_mediaBuffer.close();
    m_mediaBuffer.setData(data);
    m_mediaBuffer.open(QIODevice::ReadOnly);
    m_mediaPlayer->setMedia(QMediaContent(), &m_mediaBuffer);
    #endif

    auto useDiscord = Discord::getInstance()->enabled();

    m_mediaPlayer->setMuted(useDiscord);
    if (useDiscord) m_discordPlayer->playSound(m_fileName, data);

    m_mediaPlayer->play();
}

void SoundPlayer::onStreamManifestReceived()
{
    auto *manifest = qobject_cast<Streams::StreamManifest*>(sender());
    if (!manifest || manifest != m_streamManifest) return;

    qCDebug(gmAudioSounds()) << "Received youtube media streams:" << manifest->audio().length();

    // Find best audio stream that qmediaplayer supports
    auto audioStreams = manifest->audio();

    do
    {
        auto *stream = Streams::AudioOnlyStreamInfo::withHighestBitrate(audioStreams);

        if (stream && QMediaPlayer::hasSupport("audio/" + stream->audioCodec(),
            { stream->audioCodec() }, QMediaPlayer::StreamPlayback) > 1)
        {
            m_mediaPlayer->setMedia(QUrl(stream->url()));
            if (m_mediaPlayer->state() != QMediaPlayer::PlayingState)
                m_mediaPlayer->play();

            break;
        }
        else
        {
            audioStreams.removeOne(stream);
        }
    }
    while (!audioStreams.isEmpty());

    manifest->deleteLater();
}
