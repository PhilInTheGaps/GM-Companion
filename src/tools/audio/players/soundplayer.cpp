#include "soundplayer.h"
#include "logging.h"
#include "settings/settingsmanager.h"
#include "thirdparty/asyncfuture/asyncfuture.h"
#include "utils/fileutils.h"

#include <algorithm>
#include <cstdlib>
#include <random>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
#include <QRandomGenerator>
#endif // if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))

using namespace AsyncFuture;

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

        connect(player, &SoundPlayer::playerStopped, this, &SoundPlayerController::onPlayerStopped);
        connect(this, &SoundPlayerController::setPlayerVolume, player, &SoundPlayer::setVolume);
        connect(this, &SoundPlayerController::stopAll, player, &SoundPlayer::stop);
        connect(this, &SoundPlayerController::stopElement, player, &SoundPlayer::stopElement);

        m_players.append(player);
        player->play();

        emit soundsChanged(elements());
    }
}

void SoundPlayerController::stop(const QString &element)
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

auto SoundPlayerController::activeElements() const -> QList<QObject *>
{
    QList<QObject *> list;

    for (auto player : m_players)
    {
        if (player)
        {
            list.append(player->element());
        }
    }

    return list;
}

/**
 * @brief Check if a sound element is currently playing
 * @param element SoundElement to be checked
 * @return True if element is playing
 */
auto SoundPlayerController::isSoundPlaying(AudioElement *element) const -> bool
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
void SoundPlayerController::setVolume(int linear, int logarithmic)
{
    m_volume = logarithmic;
    emit setPlayerVolume(linear, logarithmic);
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
    player->deleteLater();

    emit soundsChanged(elements());
}

SoundPlayer::SoundPlayer(AudioElement *element, int volume, QNetworkAccessManager *networkManager,
                         DiscordPlayer *discordPlayer, QObject *parent)
    : AudioPlayer(parent), m_element(element), m_discordPlayer(discordPlayer)
{
    if (!element)
    {
        qCWarning(gmAudioSounds()) << "Error: could not initialize soundplayer, element is null";
        return;
    }

    m_mediaPlayer = new QMediaPlayer(this);
    m_mediaPlayer->setObjectName(element->name());
    m_mediaPlayer->setVolume(volume);

    connect(m_mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &SoundPlayer::onMediaStatusChanged);
    connect(m_mediaPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this,
            &SoundPlayer::onMediaPlayerError);

    m_playlist = element->files();
    applyShuffleMode();
}

SoundPlayer::~SoundPlayer()
{
    m_mediaPlayer->stop();
}

void SoundPlayer::loadMedia(AudioFile *file)
{
    qCDebug(gmAudioSounds()) << "Loading media" << file->url();

    auto useDiscord = Discord::getInstance()->enabled();

    switch (file->source())
    {
    case AudioFile::Source::File: {
        if (m_fileRequestContext) m_fileRequestContext->deleteLater();
        m_fileRequestContext = new QObject(this);

        m_fileName = file->url();
        observe(Files::File::getDataAsync(FileUtils::fileInDir(file->url(), SettingsManager::getPath("sounds"))))
            .context(m_fileRequestContext, [this](Files::FileDataResult *result) { onFileReceived(result); });
        break;
    }

    case AudioFile::Source::Web: {
        m_mediaPlayer->setMedia(QUrl(file->url()));
        m_mediaPlayer->play();
        m_mediaPlayer->setMuted(useDiscord);
        m_fileName = file->url();
        if (useDiscord) m_discordPlayer->playSound(file->url());
        break;
    }

    case AudioFile::Source::Youtube: {
        //        m_streamManifest = m_videoClient->streams()->getManifest(file->url());
        //        connect(m_streamManifest, &Streams::StreamManifest::ready, this,
        //        &SoundPlayer::onStreamManifestReceived); m_mediaPlayer->setMuted(useDiscord); m_fileName =
        //        file->url(); if (useDiscord) m_discordPlayer->playSound(file->url());
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

void SoundPlayer::stopElement(const QString &element)
{
    if (m_element && (m_element->name() == element))
    {
        qCDebug(gmAudioSounds()) << "Stopping" << element;
        m_mediaPlayer->stop();
        emit playerStopped(this);
    }
}

void SoundPlayer::setVolume(int linear, int logarithmic)
{
    Q_UNUSED(linear)
    m_mediaPlayer->setVolume(logarithmic);
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
#else  // if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, m_playlist.length() - 1);
        m_playlistIndex = dis(gen);
#endif // if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
        loadMedia(m_playlist[m_playlistIndex]);
        return;
    }

    // choose next in line (mode 0, 2, 3)
    if (m_playlistIndex + 1 < m_playlist.length())
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

#else  // if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
    std::random_device rd;
    std::mt19937 gen(rd());
    QList<AudioFile *> temp;

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

void SoundPlayer::onFileReceived(Files::FileDataResult *result)
{
    if (!result) return;

    m_mediaPlayer->stop();

    if (result->data().isEmpty())
    {
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
                qCWarning(gmAudioSounds())
                    << "Error: Could not open temporary file even after incrementing the filename" << file.fileName()
                    << file.errorString();
                result->deleteLater();
                return;
            }
        }
        else
        {
            qCWarning(gmAudioSounds()) << "Error: Could not open temporary file:" << file.fileName()
                                       << file.errorString();
            result->deleteLater();
            return;
        }
    }

    qCDebug(gmAudioSounds()) << file.fileName();
    file.write(result->data());
    file.close();

    m_mediaPlayer->setMedia(QMediaContent(QUrl::fromLocalFile(file.fileName())), nullptr);
#else
    m_mediaBuffer.close();
    m_mediaBuffer.setData(result->data());
    m_mediaBuffer.open(QIODevice::ReadOnly);
    m_mediaPlayer->setMedia(QMediaContent(), &m_mediaBuffer);
#endif

    auto useDiscord = Discord::getInstance()->enabled();

    m_mediaPlayer->setMuted(useDiscord);
    if (useDiscord) m_discordPlayer->playSound(m_fileName, result->data());

    m_mediaPlayer->play();
    result->deleteLater();
}
