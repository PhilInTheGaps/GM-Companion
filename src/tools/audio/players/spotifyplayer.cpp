#include "spotifyplayer.h"
#include "logging.h"
#include "services/spotify/spotify.h"
#include "services/spotify/spotifyutils.h"
#include "o0globals.h"
#include "utils/utils.h"
#include "settings/settingsmanager.h"

#include <QDateTime>
#include <QJsonArray>
#include <QJsonDocument>

SpotifyPlayer::SpotifyPlayer(MetaDataReader *mDReader, DiscordPlayer *discordPlayer,
                             QNetworkAccessManager *networkManager, QObject *parent)
    : AudioPlayer(parent), metaDataReader(mDReader), m_networkManager(networkManager),
      m_discordPlayer(discordPlayer)
{
    qDebug() << "Loading Spotify Tool ...";

    // Signals
    connect(Spotify::getInstance(), &Spotify::receivedReply, this, &SpotifyPlayer::gotPlaylistInfo);

    // Timer for "current song" updates
    m_timer         = new QTimer(this);
    m_periodicTimer = new QTimer(this);

    connect(m_timer, &QTimer::timeout, [ = ]()
    {
        stop();
        emit songEnded();
    });

    connect(m_periodicTimer, &QTimer::timeout, [ = ]()
    {
        getCurrentSong();
    });

    qCDebug(gmAudioSpotify) << "Spotify player loaded.";
}

SpotifyPlayer::~SpotifyPlayer()
{
    stop();
}

/**
 * @brief Play a specific playlist or album
 * @param id Spotify ID of playlist or album
 * @param offset Index offset. Playback starts with song at offset index
 */
void SpotifyPlayer::play(const QString& id, int offset, bool playOnce)
{
    qCDebug(gmAudioSpotify) << "Trying to play playlist or album ...";
    m_playOnce = playOnce;

    if (Spotify::getInstance()->isGranted())
    {
        qCDebug(gmAudioSpotify) << "Playing:" << id << "Offset:" << offset;
        m_currentIndex = offset;

        QJsonObject jo;

        if (id.contains("track:"))
        {
            QJsonArray uris;
            uris.append(id);
            jo.insert("uris", uris);
        }
        else
        {
            jo.insert("context_uri", id);
        }

        if ((offset < m_trackIdList.size()) && (offset > -1) && !id.contains("track:"))
        {
            QJsonObject jOffset;
            jOffset.insert("uri", m_trackIdList[offset]);
            jo.insert("offset", jOffset);
        }

        QJsonDocument d(jo);

        // Is discord mode enabled?
        if (Discord::getInstance()->enabled())
        {
            m_discordPlayer->playMusic(id);
        }

        QUrl url("https://api.spotify.com/v1/me/player/play");
        Spotify::getInstance()->put(url, d.toJson(QJsonDocument::JsonFormat::Compact));

        m_isPlaying = true;

        qCDebug(gmAudioSpotify) << "Emitting startedPlaying()";
        emit startedPlaying();

        m_currentIndex = m_trackIdList.indexOf(id);

        startTimer();

        QTimer::singleShot(1000, [ = ]() { getCurrentSong(); });
    }
    else
    {
        qCWarning(gmSpotify) << "Tried to play element, but access has not been granted yet.";
        if (Spotify::getInstance()->connected())
        {
            Spotify::getInstance()->grant();
        }
        else
        {
            qCWarning(gmSpotify) << "Tried to play element, but spotify connection is disabled.";
        }
    }
}

/**
 * @brief Conitinue playback
 */
void SpotifyPlayer::play()
{
    qCDebug(gmAudioSpotify) << "Continuing playback ...";

    Spotify::getInstance()->put(QUrl("https://api.spotify.com/v1/me/player/play"));
    m_isPlaying = true;
    startTimer();
    emit startedPlaying();
}

void SpotifyPlayer::startTimer(int interval)
{
    int periodicInterval = interval > 10000 ? interval / 2 : 5000;

    m_timer->stop();
    m_periodicTimer->start(periodicInterval);

    if (interval > 0)
    {
        qCDebug(gmAudioSpotify) << "Resuming timer with remaining time:" << interval;
        m_timer->start(interval);
        return;
    }
}

/**
 * @brief Stop playback
 */
void SpotifyPlayer::stop()
{
    qCDebug(gmAudioSpotify) << "Stopping playback ...";

    if (m_isPlaying)
    {
        m_timer->stop();
        m_periodicTimer->stop();
        Spotify::getInstance()->put(QUrl("https://api.spotify.com/v1/me/player/pause"));
        m_isPlaying = false;
    }
}

/**
 * @brief Toggle pause / play
 */
void SpotifyPlayer::pausePlay()
{
    if (m_isPlaying) stop();
    else play();
}

/**
 * @brief Switch to next song in playlist
 */
void SpotifyPlayer::next()
{
    qCDebug(gmAudioSpotify) << "Skipping to next track ...";

    QNetworkRequest request(QUrl("https://api.spotify.com/v1/me/player/next"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);
    Spotify::getInstance()->post(request);

    startTimer();
}

/**
 * @brief Play current song again
 */
void SpotifyPlayer::again()
{
    qCDebug(gmAudioSpotify) << "Playing track again ...";
    Spotify::getInstance()->put(QUrl("https://api.spotify.com/v1/me/player/seek?position_ms=" + QString::number(1)));
}

void SpotifyPlayer::setLinearVolume(int volume)
{
    qCDebug(gmAudioSpotify) << "Setting volume:" << volume;

    auto useDiscod = Discord::getInstance()->enabled();

    Spotify::getInstance()->put(QUrl("https://api.spotify.com/v1/me/player/volume?volume_percent=" + QString::number(useDiscod ? 0: volume)));

    m_volume = volume;
}

/**
 * @brief Get MetaData of the song that's currently playing
 */
void SpotifyPlayer::getCurrentSong()
{
    qCDebug(gmAudioSpotify) << "Getting info on current song ...";

    QNetworkRequest request(QUrl("https://api.spotify.com/v1/me/player/currently-playing"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);

    int requestId = Spotify::getInstance()->get(request);

    connect(Spotify::getInstance(), &Spotify::receivedReply, [ = ]
            (int id, QNetworkReply::NetworkError error, const QByteArray& data)
    {
        if (error != QNetworkReply::NoError)
        {
            qCWarning(gmAudioSpotify()) << "Error:" << error;
            return;
        }

        if (requestId != id) return;

        const auto root = QJsonDocument::fromJson(data).object();
        const auto item = root.value("item").toObject();

        auto *metadata = new AudioMetaData(metaDataReader);
        metadata->setTitle(item.value("name").toString());
        const auto album = item.value("album").toObject();
        metadata->setAlbum(album.value("name").toString());

        if (item.value("artists").toArray().count() > 0)
        {
            metadata->setArtist(item.value("artists").toArray()[0]
                    .toObject().value("name").toString());
        }

        if (album.value("images").toArray().count() > 0)
        {
            metadata->setCover(album.value("images").toArray()[0]
                    .toObject().value("url").toString());
        }

        int duration = item.value("duration_ms").toInt();
        int progress = root.value("progress_ms").toInt();

        startTimer(duration - progress);
        m_currentIndex = m_trackList.indexOf(metadata->title());

        metaDataReader->setMetaData(metadata);
    });
}

/**
 * @brief Get track list of current playlist or album
 */
void SpotifyPlayer::getPlaylistTracks(const QString& uri)
{
    qCDebug(gmAudioSpotify) << "Getting info on current playlist or album ...";

    QUrl url;
    QString id = SpotifyUtils::getIdFromUri(uri);

    switch (SpotifyUtils::getUriType(uri))
    {
    case 0: // Album
        url = QUrl("https://api.spotify.com/v1/albums/" + id + "/tracks");
        break;

    case 1: // Playlist
        url = QUrl("https://api.spotify.com/v1/playlists/" + id + "/tracks");
        break;

    case 2: // Track
        url = QUrl("https://api.spotify.com/v1/tracks/" + id);
        break;

    default:
        qCCritical(gmAudioSpotify()) << "Could not get tracks, unknown uri type" << uri;
        return;
    }

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);
    m_requestIdMap["getPlaylistTracks"] = Spotify::getInstance()->get(request);
}

/**
 * @brief When a GET request for a playlist or album is received
 * @param id Request ID
 * @param error Request Error
 * @param data Contains the received data in json form
 */
void SpotifyPlayer::gotPlaylistInfo(int requestId, QNetworkReply::NetworkError error, const QByteArray& data)
{
    if (!(m_requestIdMap.contains("getPlaylistTracks") && (requestId == m_requestIdMap["getPlaylistTracks"]))) return;

    qCDebug(gmAudioSpotify) << "Got playlist info." << requestId;

    if (error != QNetworkReply::NoError) qCWarning(gmAudioSpotify()) << "    Error:" << error;

    const auto root = QJsonDocument::fromJson(data).object();
    const auto href = root["href"].toString();

    m_trackList.clear();
    m_trackIdList.clear();

    if (href.contains("albums") || href.contains("playlists"))
    {
        auto id = SpotifyUtils::getIdFromHref(href);
        QList<SpotifyTrack> trackList;

        for (auto track : root["items"].toArray())
        {
            auto trackObject = track.toObject().contains("track") ? track.toObject()["track"].toObject() : track.toObject();

            if (trackObject.value("available_markets").toArray().isEmpty()) continue;

            SpotifyTrack spotifyTrack;
            spotifyTrack.title = trackObject["name"].toString();
            spotifyTrack.uri   = trackObject["uri"].toString();

            trackList.append(spotifyTrack);
            m_trackList.append(spotifyTrack.title);
            m_trackIdList.append(spotifyTrack.uri);
        }

        emit receivedPlaylistTracks(trackList, id);
    }
    else if (href.contains("tracks"))
    {
        SpotifyTrack track;
        track.title = root["name"].toString();
        track.uri   = root["uri"].toString();
        emit receivedPlaylistTracks({ track }, SpotifyUtils::getIdFromUri(track.uri));
    }
    else
    {
        qCCritical(gmAudioSpotify()) << "Received unknown track info" << data;
        return;
    }
}
