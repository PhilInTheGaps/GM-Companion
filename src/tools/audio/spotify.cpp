#include "spotify.h"
#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QByteArray>
#include <QUrlQuery>

#include "o0settingsstore.h"
#include "o2requestor.h"
#include "o0requestparameter.h"
#include "o0globals.h"

Spotify::Spotify(QObject *parent) : QObject(parent)
{
    qDebug() << "Starting Auth ...";

    m_spotify = new O2Spotify;

    // Client ID
    m_spotify->setClientId(m_sManager.getSetting(Setting::spotifyID));
    m_spotify->setClientSecret(m_sManager.getSetting(Setting::spotifySecret));

    // Scopes
    m_spotify->setScope("user-library-read playlist-read-private streaming user-modify-playback-state user-read-currently-playing "
                        "user-read-playback-state");

    // Settings Store for tokens
    // TODO: Replace with secure keychain or something similar
    O0SettingsStore *settings = new O0SettingsStore("gm-companion");
    m_spotify->setStore(settings);

    // Signals
    connect(m_spotify, &O2Spotify::linkingSucceeded, [ = ]() {
        granted();
    });

    connect(m_spotify, &O2Spotify::openBrowser, [ = ](QUrl url) {
        emit authorize(url);
    });

    // Timer for "current song" updates
    m_timer = new QTimer;
    m_timer->setInterval(5000);
    connect(m_timer, &QTimer::timeout, [ = ]() {
        getCurrentSong();
    });
}

// After linking succeeded
void Spotify::granted()
{
    emit authorized();

    switch (m_afterGranted)
    {
    case 1:
        play(m_tempId);
        break;

    default:
        fetchQueuedIcons();
        break;
    }
}

// Send a put request
void Spotify::put(QUrl url, QString params)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    O2Requestor *requestor         = new O2Requestor(manager, m_spotify, this);
    QNetworkRequest r(url);

    r.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_XFORM);
    requestor->put(r, params.toLocal8Bit());
}

// Play a specific playlist or album
void Spotify::play(QString id, int offset)
{
    if (isGranted())
    {
        qDebug() << "Playing:" << id << "Offset:" << offset;
        m_currentId    = id;
        m_currentIndex = offset;

        QJsonObject jo;
        jo.insert("context_uri", id);

        if ((offset < m_trackIdList.size()) && (offset > -1))
        {
            QJsonObject jOffset;
            jOffset.insert("uri", m_trackIdList[offset]);
            jo.insert("offset", jOffset);
        }

        QJsonDocument d;
        d.setObject(jo);

        setVolume(m_volume);
        QUrl url("https://api.spotify.com/v1/me/player/play");
        put(url, d.toJson(QJsonDocument::JsonFormat::Compact));

        m_timer->start();
        getCurrentSong();
        getCurrentPlaylist();
        m_isPlaying = true;
    }
    else
    {
        m_tempId       = id;
        m_afterGranted = 1;
        grant();
    }
}

// Conitinue playback
void Spotify::play()
{
    put(QUrl("https://api.spotify.com/v1/me/player/play"));
    m_isPlaying = true;
    m_timer->start();
    m_currentIndex = m_trackList.indexOf(m_currentSongName);
    getCurrentSong();
    getCurrentPlaylist();
}

// Stop playback
void Spotify::stop()
{
    m_timer->stop();
    put(QUrl("https://api.spotify.com/v1/me/player/pause"));
    m_isPlaying = false;
}

// Toggle pause / play
void Spotify::pausePlay()
{
    if (m_isPlaying)
    {
        stop();
        m_isPlaying = false;
    }
    else
    {
        play();
        m_isPlaying = true;
    }
}

// Start playing song at index in playlist
void Spotify::setIndex(int index)
{
    play(m_currentId, index);
}

// Switch to next song in playlist
void Spotify::next()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    O2Requestor *requestor         = new O2Requestor(manager, m_spotify, this);

    QUrl url = QUrl("https://api.spotify.com/v1/me/player/next");

    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);

    requestor->post(request, "");
}

// Play current song again
void Spotify::again()
{
    qDebug() << "Playing track again ...";
    put(QUrl("https://api.spotify.com/v1/me/player/seek?position_ms=" + QString::number(1)));
}

// Set the volume, takes linear scale
void Spotify::setVolume(int volume)
{
    put(QUrl("https://api.spotify.com/v1/me/player/volume?volume_percent=" + QString::number(volume)));
    m_volume = volume;
}

// Get MetaData of the song that's currently playing
void Spotify::getCurrentSong()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    O2Requestor *requestor         = new O2Requestor(manager, m_spotify, this);
    QNetworkRequest request(QUrl("https://api.spotify.com/v1/me/player/currently-playing"));

    request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);

    connect(requestor, &O2Requestor::finished, [ = ](int id, QNetworkReply::NetworkError error, QByteArray data) {
        const auto document = QJsonDocument::fromJson(data);
        const auto root     = document.object();
        const auto item     = root.value("item").toObject();

        const auto title = item.value("name").toString();

        const auto artists    = item.value("artists").toArray();
        const auto artist     = artists[0].toObject();
        const auto artistName = artist.value("name").toString();

        const auto album     = item.value("album").toObject();
        const auto albumName = album.value("name").toString();
        const auto images    = album.value("images").toArray();
        const auto image     = images[0].toObject();
        const auto imageUrl  = image.value("url").toString();

        m_currentSongName = title;
        m_currentIndex    = m_trackList.indexOf(title);

        emit currentSongChanged(title, artistName, albumName, imageUrl);
    });

    requestor->get(request);
}

// Get track list of current playlist or album
void Spotify::getCurrentPlaylist()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    O2Requestor *requestor         = new O2Requestor(manager, m_spotify, this);
    QString id                     = m_currentId;

    bool isPlaylist = id.contains("playlist");

    if (isPlaylist)
    {
        QString user          = id.replace("spotify:user:", "");
        QString replaceString = "spotify:user:";

        user = user.left(user.indexOf(":"));
        QString playlist = id.replace(replaceString, "").replace(user, "").replace(":playlist:", "");

        QNetworkRequest request(QUrl("https://api.spotify.com/v1/playlists/" + playlist));
        request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);

        connect(requestor, &O2Requestor::finished, [ = ](int id, QNetworkReply::NetworkError error, QByteArray data) {
            const auto document    = QJsonDocument::fromJson(data);
            const auto root        = document.object();
            const auto tracks      = root.value("tracks").toObject();
            const auto tracksArray = tracks.value("items").toArray();

            m_trackList.clear();
            m_trackIdList.clear();

            for (auto trackPl : tracksArray)
            {
                const auto track = trackPl.toObject().value("track").toObject();
                m_trackList.append(track.value("name").toString());
                m_trackIdList.append(track.value("uri").toString());
            }

            emit currentPlaylistChanged(m_trackList);
        });

        requestor->get(request);
    }
    else // Album
    {
        QString album = id.replace("spotify:album:", "");

        QNetworkRequest request(QUrl("https://api.spotify.com/v1/albums/" + album + "/tracks"));
        request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);

        connect(requestor, &O2Requestor::finished, [ = ](int id, QNetworkReply::NetworkError error, QByteArray data) {
            const auto document    = QJsonDocument::fromJson(data);
            const auto root        = document.object();
            const auto tracksArray = root.value("items").toArray();

            m_trackList.clear();
            m_trackIdList.clear();

            qDebug() << tracksArray;

            for (auto trackO : tracksArray)
            {
                qDebug() << trackO.toObject();
                m_trackList.append(trackO.toObject().value("name").toString());
                m_trackIdList.append(trackO.toObject().value("uri").toString());
            }

            emit currentPlaylistChanged(m_trackList);
        });

        requestor->get(request);
    }
}

void Spotify::fetchIcon(QString id, int index)
{
    if (isGranted())
    {
        qDebug() << "Fetching thumbnail ..." << id;

        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        O2Requestor *requestor         = new O2Requestor(manager, m_spotify, this);

        if (id.contains("album:"))
        {
            QUrl url = QUrl("https://api.spotify.com/v1/albums/" + id.replace("spotify:album:", ""));

            QNetworkRequest request(url);
            request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);

            connect(requestor, &O2Requestor::finished, [ = ](int id, QNetworkReply::NetworkError error, QByteArray data) {
                qDebug() << "FINISHED" << id;

                const auto document = QJsonDocument::fromJson(data);
                const auto root     = document.object();
                const auto images   = root.value("images").toArray();
                const auto image    = images.at(0).toObject();

                QString url = image.value("url").toString();
                qDebug() << url;
                emit iconChanged(index, url);
            });

            requestor->get(request);
        }
        else
        {
            QString user = id.replace("spotify:user:", "");
            user = user.left(user.indexOf(":"));
            qDebug() << user;

            QString replaceString = "spotify:user:";
            QString playlist      = id.replace(replaceString, "").replace(user, "").replace(":playlist:", "");
            qDebug() << playlist;

            QUrl url = QUrl("https://api.spotify.com/v1/users/" + user + "/playlists/" + playlist + "/images");

            QNetworkRequest request(url);
            request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);

            connect(requestor, &O2Requestor::finished, [ = ](int id, QNetworkReply::NetworkError error, QByteArray data) {
                const auto document = QJsonDocument::fromJson(data);
                const auto root     = document.array();
                const auto image    = root.at(0).toObject();

                QString url = image.value("url").toString();
                qDebug() << url;
                emit iconChanged(index, url);
            });

            requestor->get(request);
        }
    }
    else
    {
        Playlist p;
        p.index = index;
        p.id    = id;

        iconFetchQueue.enqueue(p);
    }
}

void Spotify::fetchQueuedIcons()
{
    while (!iconFetchQueue.isEmpty())
    {
        Playlist p = iconFetchQueue.dequeue();
        fetchIcon(p.id, p.index);
    }
}
