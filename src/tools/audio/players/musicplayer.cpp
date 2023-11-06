#include "musicplayer.h"
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmAudioMusic, "gm.audio.music")

MusicPlayer::MusicPlayer(QNetworkAccessManager &networkManager, MetaDataReader &metaDataReader, QObject *parent)
    : BufferedAudioPlayer(u"music"_s, networkManager, parent), m_spotifyPlayer(metaDataReader)
{
    connect(&m_spotifyPlayer, &SpotifyPlayer::songEnded, this, &MusicPlayer::onSpotifySongEnded);
    connect(&m_spotifyPlayer, &SpotifyPlayer::stateChanged, this, &MusicPlayer::onSpotifyStateChanged);
    connect(this, &MusicPlayer::stateChanged, this, &MusicPlayer::onStateChanged);
    connect(
        this, &MusicPlayer::currentFileChanged, this,
        [&metaDataReader](const QString &path, const QByteArray &data) { metaDataReader.loadMetaData(path, data); });
}

void MusicPlayer::play(AudioElement *element)
{
    m_spotifyPlayer.stop();

    BufferedAudioPlayer::play(element);
}

void MusicPlayer::play()
{
    if (fileSource() == AudioFile::Source::Spotify)
    {
        m_spotifyPlayer.play();
        state(State::Playing);
        return;
    }

    BufferedAudioPlayer::play();
}

void MusicPlayer::pause()
{
    if (fileSource() == AudioFile::Source::Spotify)
    {
        m_spotifyPlayer.pause();
        return;
    }

    BufferedAudioPlayer::pause();
}

void MusicPlayer::stop()
{
    m_spotifyPlayer.stop();

    BufferedAudioPlayer::stop();
}

void MusicPlayer::again()
{
    if (fileSource() == AudioFile::Source::Spotify)
    {
        m_spotifyPlayer.again();
        return;
    }

    BufferedAudioPlayer::again();
}

void MusicPlayer::setVolume(int linear, int logarithmic)
{
    BufferedAudioPlayer::setVolume(linear, logarithmic);
    m_spotifyPlayer.setVolume(linear, logarithmic);
}

void MusicPlayer::handleUnsupportedMediaSource(const AudioFile &file)
{
    switch (file.source())
    {
    case AudioFile::Source::Spotify:
        loadSpotifyFile(file);
        break;
    case AudioFile::Source::Youtube:
        loadYoutubeFile(file);
        break;
    default:
        qCCritical(gmAudioMusic()) << "loadMedia() is not implemented for type" << file.source();
        next();
        break;
    }
}

void MusicPlayer::loadSpotifyFile(const AudioFile &file)
{
    m_spotifyPlayer.play(file.url());
    state(State::Playing);
}

void MusicPlayer::loadYoutubeFile(const AudioFile &file) const
{
    Q_UNUSED(file)
    qCDebug(gmAudioMusic) << "Media is a youtube video ...";
    qCCritical(gmAudioMusic()) << "Youtube integration is currently broken";
}

void MusicPlayer::onSpotifySongEnded()
{
    qCDebug(gmAudioMusic()) << "Spotify song ended, starting next song ...";
    next();
}

void MusicPlayer::onSpotifyStateChanged(State state)
{
    if (fileSource() != AudioFile::Source::Spotify) return;

    this->state(state);
}

void MusicPlayer::onStateChanged(State state)
{
    if (fileSource() == AudioFile::Source::Spotify) return;

    switch (state)
    {
    case AudioPlayer::State::Playing:
    case AudioPlayer::State::Loading:
        m_spotifyPlayer.stop();
        break;
    default:
        break;
    }
}
