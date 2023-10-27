#include "spotifyplayer.h"
#include "services/spotify/spotify.h"
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(gmAudioSpotify, "gm.audio.spotify")

using namespace Services;

SpotifyPlayer::SpotifyPlayer(MetaDataReader &mDReader, QObject *parent)
    : AudioPlayer(parent), m_metaDataReader(mDReader)
{
    qCDebug(gmAudioSpotify()) << "Loading ...";

    // Timer for "current song" updates
    m_metaDataTimer.setSingleShot(true);

    connect(&m_songDurationTimer, &QTimer::timeout, this, &SpotifyPlayer::onDurationTimerTimeout);
    connect(&m_metaDataTimer, &QTimer::timeout, this, &SpotifyPlayer::onMetaDataTimerTimeout);
}

/// The current song has ended, stop any spotify activity and notify music player
void SpotifyPlayer::onDurationTimerTimeout()
{
    stop();
    emit songEnded();
}

/// Periodically fetch meta data from spotify
void SpotifyPlayer::onMetaDataTimerTimeout()
{
    getCurrentSong();
}

auto SpotifyPlayer::isSpotifyAvailable() -> bool
{
    if (Spotify::instance()->isGranted()) return true;

    if (Spotify::instance()->connected())
    {
        qCWarning(gmAudioSpotify) << "Spotify is available, but access has not been granted yet.";
        Spotify::instance()->grant();
        return false;
    }

    qCWarning(gmAudioSpotify) << "Spotify connection is disabled.";
    return false;
}

/**
 * @brief Play a specific spotify element
 * @param id Spotify ID of element
 * @param offset Index offset. Playback starts with song at offset index
 */
void SpotifyPlayer::play(const QString &uri)
{
    if (!isSpotifyAvailable()) return;

    qCDebug(gmAudioSpotify) << "Playing:" << uri;
    m_currentUri = uri;

    const auto callback = [this](RestReply &&reply) {
        if (reply.hasError())
        {
            qCWarning(gmAudioSpotify()) << reply.errorText();
            emit songEnded();
            return;
        }

        m_metaDataReader.clearMetaData();
        startMetaDataTimer();

        state(AudioPlayer::State::Playing);
    };

    Spotify::instance()->player.play(uri).then(this, callback);

    state(AudioPlayer::State::Loading);
}

/**
 * @brief Continue playback
 */
void SpotifyPlayer::play()
{
    if (!isSpotifyAvailable() || state() == AudioPlayer::State::Playing) return;

    qCDebug(gmAudioSpotify) << "Continuing playback ...";

    Spotify::instance()->player.play().then(this, [this](const RestReply &reply) {
        if (reply.hasError())
        {
            qCWarning(gmAudioSpotify()) << "Could not continue playback:" << reply.errorText();
            return;
        }
        startMetaDataTimer();
    });

    state(AudioPlayer::State::Playing);
}

void SpotifyPlayer::startDurationTimer(std::chrono::milliseconds interval)
{
    m_songDurationTimer.stop();

    qCDebug(gmAudioSpotify) << "Resuming timer with remaining time:" << interval.count() << "ms";
    m_songDurationTimer.start(interval);
}

void SpotifyPlayer::startMetaDataTimer()
{
    m_metaDataTimer.start(std::chrono::seconds(1));
}

void SpotifyPlayer::pause()
{
    if (state() != AudioPlayer::State::Playing) return;

    qCDebug(gmAudioSpotify) << "Pausing playback ...";

    m_songDurationTimer.stop();
    m_metaDataTimer.stop();

    Spotify::instance()->player.pause().then(this, [](const RestReply &reply) {
        if (reply.hasError())
        {
            qCWarning(gmAudioSpotify()) << "Could not pause playback:" << reply.errorText();
        }
    });

    state(AudioPlayer::State::Paused);
}

void SpotifyPlayer::stop()
{
    pause();
    state(AudioPlayer::State::Stopped);
}

/**
 * @brief Toggle pause / play
 */
void SpotifyPlayer::pausePlay()
{
    if (state() == AudioPlayer::State::Playing)
    {
        pause();
        return;
    }

    play();
}

/**
 * @brief Switch to next song in playlist
 */
void SpotifyPlayer::next()
{
    qCDebug(gmAudioSpotify) << "Skipping to next track ...";

    Spotify::instance()->player.next().then(this, [this](const RestReply &reply) {
        if (reply.hasError())
        {
            qCWarning(gmAudioSpotify()) << "Could not skip track:" << reply.errorText();
            return;
        }

        startMetaDataTimer();
    });
}

/**
 * @brief Play current song again
 */
void SpotifyPlayer::again()
{
    qCDebug(gmAudioSpotify) << "Playing track again ...";

    Spotify::instance()->player.seek(1).then(this, [](const RestReply &reply) {
        if (reply.hasError())
        {
            qCWarning(gmAudioSpotify()) << "Could not play track again:" << reply.errorText();
        }
    });
}

void SpotifyPlayer::setVolume(int linear, int logarithmic)
{
    Q_UNUSED(linear)
    qCDebug(gmAudioSpotify) << "Setting volume:" << logarithmic;

    auto _ = Spotify::instance()->player.volume(logarithmic);

    m_volume = logarithmic;
}

/**
 * @brief Get MetaData of the song that's currently playing
 */
void SpotifyPlayer::getCurrentSong()
{
    qCDebug(gmAudioSpotify) << "Getting info on current song ...";

    const auto callback = [this](SpotifyCurrentTrack &&track) {
        if (track.track.uri != m_currentUri)
        {
            qCDebug(gmAudioSpotify()) << "Found track" << track.track.uri << "is not the expected track"
                                      << m_currentUri;
            startMetaDataTimer();
            return;
        }

        AudioMetaData metadata;
        metadata.title(track.track.name);
        metadata.album(track.track.album.name);
        metadata.artist(track.track.artistNames());
        metadata.cover(track.track.image().url);

        auto duration = track.track.duration;
        auto progress = track.progress;

        startDurationTimer(duration - progress);
        m_metaDataReader.setMetaData(metadata);
    };

    Spotify::instance()->player.getCurrentlyPlaying().then(this, callback);
}
