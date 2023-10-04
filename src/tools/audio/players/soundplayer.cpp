#include "soundplayer.h"
#include "../project/audioelement.h"
#include "filesystem/file.h"
#include "settings/settingsmanager.h"
#include "utils/fileutils.h"
#include <QLoggingCategory>
#include <QRandomGenerator>
#include <algorithm>
#include <cstdlib>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmAudioSounds, "gm.audio.sounds")

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
        m_fileRequestContext = std::make_unique<QObject>();

        m_fileName = file->url();
        Files::File::getDataAsync(FileUtils::fileInDir(file->url(), SettingsManager::getPath(u"sounds"_s)))
            .then(m_fileRequestContext.get(),
                  [this](std::shared_ptr<Files::FileDataResult> result) { onFileReceived(result); });
        break;
    }

    case AudioFile::Source::Web: {
        m_mediaPlayer.setSource(QUrl(file->url()));
        m_mediaPlayer.play();
        m_audioOutput.setMuted(false);
        m_fileName = file->url();
        break;
    }

    case AudioFile::Source::Youtube:
        // FIXME
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

void SoundPlayer::onFileReceived(std::shared_ptr<Files::FileDataResult> result)
{
    if (!result) return;

    m_mediaPlayer.stop();

    if (result->data().isEmpty())
    {
        next();
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
                return;
            }
        }
        else
        {
            qCWarning(gmAudioSounds()) << "Error: Could not open temporary file:" << file.fileName()
                                       << file.errorString();
            return;
        }
    }

    qCDebug(gmAudioSounds()) << file.fileName();
    file.write(result->data());
    file.close();

    m_mediaPlayer.setSource(QUrl::fromLocalFile(file.fileName()));
#else
    if (m_mediaBuffer)
    {
        m_mediaBuffer->close();
    }

    m_mediaBuffer = std::make_unique<QBuffer>();
    m_mediaBuffer->setData(result->data());
    m_mediaBuffer->open(QIODevice::ReadOnly);
    m_mediaPlayer.setSourceDevice(m_mediaBuffer.get());
#endif

    m_audioOutput.setMuted(false);

    m_mediaPlayer.play();
}
