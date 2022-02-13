#include "spotifyplayer.h"
#include "services/spotify/spotify.h"
#include "services/spotify/spotifyutils.h"
#include "o0globals.h"
#include "utils/utils.h"
#include "settings/settingsmanager.h"
#include "thirdparty/asyncfuture/asyncfuture.h"

#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLoggingCategory>
#include <string>

Q_LOGGING_CATEGORY(gmAudioSpotify, "gm.audio.spotify")

using namespace AsyncFuture;

SpotifyPlayer::SpotifyPlayer(MetaDataReader *mDReader, DiscordPlayer *discordPlayer, QObject *parent)
    : AudioPlayer(parent), metaDataReader(mDReader), m_discordPlayer(discordPlayer)
{
    qCDebug(gmAudioSpotify()) << "Loading ...";

    // Timer for "current song" updates
    m_songDurationTimer = new QTimer(this);
    m_metaDataTimer = new QTimer(this);
    m_metaDataTimer->setSingleShot(true);

    connect(m_songDurationTimer, &QTimer::timeout, this, &SpotifyPlayer::onDurationTimerTimeout);
    connect(m_metaDataTimer, &QTimer::timeout, this, &SpotifyPlayer::onMetaDataTimerTimeout);
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
void SpotifyPlayer::play(const QString& uri)
{
    if (!isSpotifyAvailable()) return;

    qCDebug(gmAudioSpotify) << "Playing:" << uri;
    m_currentUri = uri;

    // Is discord mode enabled?
    if (Discord::getInstance()->enabled())
    {
        m_discordPlayer->playMusic(uri);
    }

    const auto callback = [this](RestNetworkReply *reply) {
        if (!reply) return;

        if (reply->hasError())
        {
            qCWarning(gmAudioSpotify()) << reply->errorText();
            reply->deleteLater();
            emit songEnded();
            return;
        }

        reply->deleteLater();

        metaDataReader->clearMetaData();
        startDurationTimer({});
        startMetaDataTimer();
    };

    observe(Spotify::instance()->player->play(uri)).subscribe(callback);

    m_isPlaying = true;
    emit startedPlaying();
}

/**
 * @brief Conitinue playback
 */
void SpotifyPlayer::play()
{
    if (!isSpotifyAvailable()) return;

    qCDebug(gmAudioSpotify) << "Continuing playback ...";

    observe(Spotify::instance()->player->play()).subscribe([this](RestNetworkReply *reply) {
        if (reply) reply->deleteLater();

        startDurationTimer({});
        startMetaDataTimer();
    });

    m_isPlaying = true;
    emit startedPlaying();
}

void SpotifyPlayer::startDurationTimer(std::chrono::milliseconds interval)
{
    m_songDurationTimer->stop();

    qCDebug(gmAudioSpotify) << "Resuming timer with remaining time:" << interval.count() << "ms";
    m_songDurationTimer->start(interval);
}

void SpotifyPlayer::startMetaDataTimer()
{
    m_metaDataTimer->start(std::chrono::seconds(1));
}

/**
 * @brief Stop playback
 */
void SpotifyPlayer::stop()
{
    qCDebug(gmAudioSpotify) << "Stopping playback ...";

    if (!m_isPlaying) return;

    m_songDurationTimer->stop();
    m_metaDataTimer->stop();

    observe(Spotify::instance()->player->pause()).subscribe([](RestNetworkReply *reply) {
        if (reply) reply->deleteLater();
    });

    m_isPlaying = false;
}

/**
 * @brief Toggle pause / play
 */
void SpotifyPlayer::pausePlay()
{
    if (m_isPlaying)
    {
        stop();
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

    observe(Spotify::instance()->player->next()).subscribe([](RestNetworkReply *reply) {
        if (reply) reply->deleteLater();
    });

    startDurationTimer({});
}

/**
 * @brief Play current song again
 */
void SpotifyPlayer::again()
{
    qCDebug(gmAudioSpotify) << "Playing track again ...";

    observe(Spotify::instance()->player->seek(1)).subscribe([](RestNetworkReply *reply) {
        if (reply) reply->deleteLater();
    });
}

void SpotifyPlayer::setVolume(int linear, int logarithmic)
{
    Q_UNUSED(logarithmic)
    qCDebug(gmAudioSpotify) << "Setting volume:" << linear;

    const auto useDiscod = Discord::getInstance()->enabled();
    const auto volumePercent = useDiscod ? 0: linear;

    observe(Spotify::instance()->player->volume(volumePercent)).subscribe([](RestNetworkReply *reply) {
        if (reply) reply->deleteLater();
    });

    m_volume = linear;
}

/**
 * @brief Get MetaData of the song that's currently playing
 */
void SpotifyPlayer::getCurrentSong()
{
    qCDebug(gmAudioSpotify) << "Getting info on current song ...";

    const auto callback = [this](QSharedPointer<SpotifyCurrentTrack> track)
    {
        if (track->track->uri != m_currentUri)
        {
            startMetaDataTimer();
            return;
        }

        auto *metadata = new AudioMetaData(metaDataReader);
        metadata->setTitle(track->track->name);
        metadata->setAlbum(track->track->album->name);
        metadata->setArtist(track->track->artistString());
        metadata->setCover(track->track->image()->url);

        using namespace std::chrono;
        auto duration = milliseconds(track->track->durationMs);
        auto progress = milliseconds(track->progressMs);

        startDurationTimer(duration - progress);
        metaDataReader->setMetaData(metadata);
    };

    observe(Spotify::instance()->player->getCurrentlyPlaying()).subscribe(callback);
}
