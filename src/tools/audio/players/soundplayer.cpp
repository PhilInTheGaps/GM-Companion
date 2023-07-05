#include "soundplayer.h"
#include "settings/settingsmanager.h"
#include "thirdparty/asyncfuture/asyncfuture.h"
#include "utils/fileutils.h"
#include <QLoggingCategory>
#include <QRandomGenerator>
#include <algorithm>
#include <cstdlib>

using namespace AsyncFuture;

Q_LOGGING_CATEGORY(gmAudioSounds, "gm.audio.sounds")

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
    emit stopElement(element);
}

auto SoundPlayerController::activeElements() const -> QList<QObject *>
{
    QList<QObject *> list;
    list.reserve(m_players.size());

    foreach (const auto *player, m_players)
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

    foreach (const auto *player, m_players)
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

    foreach (const auto *player, m_players)
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

SoundPlayer::SoundPlayer(AudioElement *element, int volume, QObject *parent) : AudioPlayer(parent), m_element(element)
{
    if (!element)
    {
        qCWarning(gmAudioSounds()) << "Error: could not initialize soundplayer, element is null";
        return;
    }

    m_mediaPlayer.setObjectName(element->name());
    m_mediaPlayer.setAudioOutput(&m_audioOutput);
    m_audioOutput.setVolume(normalizeVolume(volume));

    connect(&m_mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &SoundPlayer::onMediaStatusChanged);
    connect(&m_mediaPlayer, &QMediaPlayer::errorOccurred, this, &SoundPlayer::onMediaPlayerErrorOccurred);

    m_playlist = element->files();
    applyShuffleMode();
}

void SoundPlayer::loadMedia(const AudioFile *file)
{
    qCDebug(gmAudioSounds()) << "Loading media" << file->url();

    switch (file->source())
    {
    case AudioFile::Source::File: {
        if (m_fileRequestContext) m_fileRequestContext->deleteLater();
        m_fileRequestContext = new QObject(this);

        m_fileName = file->url();
        observe(Files::File::getDataAsync(
                    FileUtils::fileInDir(file->url(), SettingsManager::getPath(QStringLiteral("sounds")))))
            .context(m_fileRequestContext, [this](Files::FileDataResult *result) { onFileReceived(result); });
        break;
    }

    case AudioFile::Source::Web: {
        m_mediaPlayer.setSource(QUrl(file->url()));
        m_mediaPlayer.play();
        m_audioOutput.setMuted(false);
        m_fileName = file->url();
        break;
    }

    case AudioFile::Source::Youtube: {
        // FIXME
        break;
    }

    default:
        qCWarning(gmAudioSounds()) << "Media type" << file->source() << "is currently not supported.";
    }
}

void SoundPlayer::play()
{
    if (m_element->mode() == AudioElement::Mode::Random)
    {
        next();
    }
    else
    {
        loadMedia(m_playlist.constFirst());
    }
}

void SoundPlayer::pause()
{
    m_mediaPlayer.pause();
}

void SoundPlayer::stop()
{
    m_mediaPlayer.stop();
    emit playerStopped(this);
}

void SoundPlayer::stopElement(const QString &element)
{
    if (m_element && (m_element->name() == element))
    {
        qCDebug(gmAudioSounds()) << "Stopping" << element;
        m_mediaPlayer.stop();
        emit playerStopped(this);
    }
}

void SoundPlayer::setVolume(int linear, int logarithmic)
{
    Q_UNUSED(linear)
    m_audioOutput.setVolume(normalizeVolume(logarithmic));
}

void SoundPlayer::next()
{
    if (!m_element || (m_playlist.isEmpty()))
    {
        emit playerStopped(this);
        return;
    }

    // Complete random
    if (m_element->mode() == AudioElement::Mode::Random)
    {
        m_playlistIndex = QRandomGenerator::system()->bounded(m_playlist.length());
        loadMedia(m_playlist.at(m_playlistIndex));
        return;
    }

    // choose next in line (mode 0, 2, 3)
    if (m_playlistIndex + 1 < m_playlist.length())
    {
        loadMedia(m_playlist.at(++m_playlistIndex));
    }

    // loop around (Mode 0 or 2)
    else if (m_element->mode() != AudioElement::Mode::ListOnce)
    {
        m_playlistIndex = 0;
        loadMedia(m_playlist.constFirst());
    }

    // reached end of playlist, stop
    else
    {
        emit playerStopped(this);
    }
}

void SoundPlayer::applyShuffleMode()
{
    if (m_element->mode() != AudioElement::Mode::RandomList) return;

    QList<AudioFile *> temp;

    while (!m_playlist.isEmpty())
    {
        temp.append(m_playlist.takeAt(QRandomGenerator::global()->bounded(m_playlist.size())));
    }
    m_playlist = temp;
}

void SoundPlayer::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    qCDebug(gmAudioSounds()) << "Status changed:" << status;

    if (status == QMediaPlayer::EndOfMedia)
    {
        next();
    }
}

void SoundPlayer::onMediaPlayerErrorOccurred(QMediaPlayer::Error error, const QString &errorString)
{
    qCWarning(gmAudioSounds()) << error << errorString;

    if (error == QMediaPlayer::FormatError)
    {
        next();
    }
}

void SoundPlayer::onFileReceived(Files::FileDataResult *result)
{
    if (!result) return;

    m_mediaPlayer.stop();

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

    m_mediaPlayer.setSource(QUrl::fromLocalFile(file.fileName()));
#else
    m_mediaBuffer.close();
    m_mediaBuffer.setData(result->data());
    m_mediaBuffer.open(QIODevice::ReadOnly);
    m_mediaPlayer.setSourceDevice(&m_mediaBuffer);
#endif

    m_audioOutput.setMuted(false);

    m_mediaPlayer.play();
    result->deleteLater();
}
