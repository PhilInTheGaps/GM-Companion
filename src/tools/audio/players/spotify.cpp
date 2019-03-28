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

Spotify::Spotify(FileManager *fManager, MetaDataReader *mDReader)
{
    qDebug() << "Loading Spotify Tool ...";

    m_spotify      = new O2Spotify;
    fileManager    = fManager;
    metaDataReader = mDReader;
    m_manager      = new QNetworkAccessManager;

    // Scopes
    m_spotify->setScope("user-library-read playlist-read-private streaming user-modify-playback-state user-read-currently-playing "
                        "user-read-playback-state");
    m_spotify->setLocalPort(59991);

    // Settings Store for tokens
    // TODO: Replace with secure keychain or something similar
    O0SettingsStore *settings = new O0SettingsStore("gm-companion");
    m_spotify->setStore(settings);

    // Signals
    connect(m_spotify, &O2Spotify::linkingSucceeded, [ = ]() { if (m_spotify->linked()) { m_waitingForAuth = false; granted(); } });
    connect(m_spotify, &O2Spotify::openBrowser,      [ = ](QUrl url) { m_authUrl = url; m_waitingForAuth = true; QDesktopServices::openUrl(url); emit authorize(url); });

    // Timer for "current song" updates
    m_timer = new QTimer;
    connect(m_timer, &QTimer::timeout, [ = ]() { getCurrentSong(); });

    grant();
    qDebug() << "Spotify Tool loaded.";
}

void Spotify::grant()
{
    qDebug() << "SPOTIFY: Granting access ...";

    QString id     = m_sManager.getSetting(Setting::spotifyID);
    QString secret = m_sManager.getSetting(Setting::spotifySecret);

    // Client ID and Secret
    m_spotify->setClientId(id);
    m_spotify->setClientSecret(secret);

    if (!id.isEmpty() && !secret.isEmpty())
    {
        qDebug() << "SPOTIFY: Trying to link ...";
        m_spotify->link();
    }
    else
    {
        qDebug() << "SPOTIFY ERROR: Client ID or Secret is empty!";
    }
}

/**
 * @brief After linking with Spotify succeeded
 */
void Spotify::granted()
{
    qDebug() << "SPOTIFY: Access has been granted!";
    emit authorized();

    fetchQueuedIcons();
}

/**
 * @brief Send a PUT request
 * @param url URL to send request to
 * @param params additional parameters
 */
void Spotify::put(QUrl url, QString params)
{
    qDebug() << "SPOTIFY: Sending PUT Request to URL" << url << "and parameters:" << params;

    O2Requestor *requestor = new O2Requestor(m_manager, m_spotify, this);
    QNetworkRequest r(url);

    r.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_XFORM);

    connect(requestor, qOverload<int, QNetworkReply::NetworkError, QByteArray>(&O2Requestor::finished), [](int id, QNetworkReply::NetworkError error, QByteArray data) {
        qDebug() << "Completed PUT Request. ID:" << id << "Error:" << error;
        qDebug() << "Reply:";
        qDebug() << data;
    });

    requestor->put(r, params.toLocal8Bit());
}

/**
 * @brief Play a specific playlist or album
 * @param id Spotify ID of playlist or album
 * @param offset Index offset. Playback starts with song at offset index
 */
void Spotify::play(QString id, int offset)
{
    qDebug() << "SPOTIFY: Trying to play playlist or album ...";

    if (isGranted())
    {
        qDebug() << "SPOTIFY: Playing:" << id << "Offset:" << offset;
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

        m_timer->stop();
        m_timer->start(3000);
        getCurrentPlaylist();
        m_isPlaying = true;

        qDebug() << "SPOTIFY: Emitting startedPlaying()";
        emit startedPlaying();
    }
    else
    {
        qDebug() << "SPOTIFY: Tried to play element, but access has not been granted yet.";
        grant();
    }
}

/**
 * @brief Conitinue playback
 */
void Spotify::play()
{
    qDebug() << "SPOTIFY: Continuing playback ...";

    put(QUrl("https://api.spotify.com/v1/me/player/play"));
    m_isPlaying = true;
    m_timer->start();
    getCurrentSong();
    getCurrentPlaylist();

    qDebug() << "SPOTIFY: Emitting startedPlaying()";
    emit startedPlaying();
}

/**
 * @brief Stop playback
 */
void Spotify::stop()
{
    qDebug() << "SPOTIFY: Stopping playback ...";

    if (m_isPlaying)
    {
        m_timer->stop();
        put(QUrl("https://api.spotify.com/v1/me/player/pause"));
        m_isPlaying = false;
    }
}

/**
 * @brief Toggle pause / play
 */
void Spotify::pausePlay()
{
    if (m_isPlaying) stop();

    else play();
}

/**
 * @brief Switch to next song in playlist
 */
void Spotify::next()
{
    qDebug() << "SPOTIFY: Skipping to next track ...";

    O2Requestor *requestor = new O2Requestor(m_manager, m_spotify, this);

    QUrl url = QUrl("https://api.spotify.com/v1/me/player/next");

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);
    requestor->post(request, "");

    m_timer->setInterval(3000);
}

/**
 * @brief Play current song again
 */
void Spotify::again()
{
    qDebug() << "SPOTIFY: Playing track again ...";
    put(QUrl("https://api.spotify.com/v1/me/player/seek?position_ms=" + QString::number(1)));
}

/**
 * @brief Set the volume, takes linear scale
 * @param volume Volume value from 0 to 100
 */
void Spotify::setVolume(int volume)
{
    qDebug() << "SPOTIFY: Setting volume:" << volume;
    put(QUrl("https://api.spotify.com/v1/me/player/volume?volume_percent=" + QString::number(volume)));
    m_volume = volume;
}

/**
 * @brief Get MetaData of the song that's currently playing
 */
void Spotify::getCurrentSong()
{
    qDebug() << "SPOTIFY: Getting info on current song ...";

    O2Requestor *requestor = new O2Requestor(m_manager, m_spotify, this);
    QNetworkRequest request(QUrl("https://api.spotify.com/v1/me/player/currently-playing"));

    request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);

    connect(requestor, qOverload<int, QNetworkReply::NetworkError, QByteArray>(&O2Requestor::finished), [ = ](int id, QNetworkReply::NetworkError error, QByteArray data) {
        const auto root = QJsonDocument::fromJson(data).object();
        const auto item = root.value("item").toObject();

        MetaData m;
        m.type           = tr("Spotify") + ": " + currentElement->name();
        m.title          = item.value("name").toString();
        m.artist         = item.value("artists").toArray()[0].toObject().value("name").toString();
        const auto album = item.value("album").toObject();
        m.album          = album.value("name").toString();
        m.cover          = album.value("images").toArray()[0].toObject().value("url").toString();

        int duration = item.value("duration_ms").toInt();
        int progress = root.value("progress_ms").toInt();

        m_timer->stop();

        // So MetaData will be updated 3 seconds after
        // the current song has ended
        m_timer->start(duration - progress + 3000);

        m_currentIndex = m_trackList.indexOf(m.title);

        metaDataReader->setMetaData(m);
    });

    requestor->get(request);
}

/**
 * @brief Get track list of current playlist or album
 */
void Spotify::getCurrentPlaylist()
{
    qDebug() << "SPOTIFY: Getting info on current playlist or album ...";

    O2Requestor *requestor = new O2Requestor(m_manager, m_spotify, this);
    QString id             = m_currentId;
    QUrl    url;

    if (id.contains("playlist")) // Element is playlist
    {
        QString playlist = id.right(id.length() - id.lastIndexOf(':') - 1);
        url = QUrl("https://api.spotify.com/v1/playlists/" + playlist);
    }
    else // Album
    {
        QString album = id.replace("spotify:album:", "");
        url = QUrl("https://api.spotify.com/v1/albums/" + album + "/tracks");
    }

    connect(requestor, qOverload<int, QNetworkReply::NetworkError, QByteArray>(&O2Requestor::finished), this, &Spotify::gotCurrentPlaylist);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);
    requestor->get(request);
}

/**
 * @brief When a GET request for a playlist or album is received
 * @param id Request ID
 * @param error Request Error
 * @param data Contains the received data in json form
 */
void Spotify::gotCurrentPlaylist(int id, QNetworkReply::NetworkError error, QByteArray data)
{
    qDebug() << "SPOTIFY: Got current playlist." << id;
    qDebug() << "    Error:" << error;

    const auto doc  = QJsonDocument::fromJson(data);
    const auto root = doc.object();
    bool isPlaylist = false;
    QJsonArray tracks;

    if (root.value("items").isArray())
    {
        qDebug() << "SPOTIFY: Current playlist is an album!";
        tracks = root.value("items").toArray();
    }
    else
    {
        qDebug() << "SPOTIFY: Current playlist is a playlist!";
        tracks     = root.value("tracks").toObject().value("items").toArray();
        isPlaylist = true;
    }

    m_trackList.clear();
    m_trackIdList.clear();

    for (auto t : tracks)
    {
        if (isPlaylist)
        {
            m_trackList.append(t.toObject().value("track").toObject().value("name").toString());
            m_trackIdList.append(t.toObject().value("track").toObject().value("uri").toString());
        }
        else
        {
            m_trackList.append(t.toObject().value("name").toString());
            m_trackIdList.append(t.toObject().value("uri").toString());
        }
    }

    emit songNamesChanged();
}

/**
 * @brief Get the URLs of the element icons in a scenario
 * @param scenario Scenario to find icons in
 */
void Spotify::fetchIcons(AudioScenario *scenario)
{
    qDebug() << "SPOTIFY: Fetching element icons ...";

    qDebug() << "Linked:" << m_spotify->linked();

    if (!scenario) return;

    for (auto a : scenario->spotifyElements())
    {
        if (!a->hasIcon() && !iconFetchQueue.contains(a)) fetchIcon(a);
    }
}

/**
 * @brief Get the icon of a spotify element
 * @param SpotifyElement to get icon of
 */
void Spotify::fetchIcon(SpotifyElement *element)
{
    if (!element) return;

    if (isGranted())
    {
        qDebug() << "SPOTIFY: Fetching thumbnail ..." << element->name();

        O2Requestor *requestor = new O2Requestor(m_manager, m_spotify, this);
        QUrl url;
        QString id = element->id();

        if (id.contains("album:")) // Element is album
        {
            url = QUrl("https://api.spotify.com/v1/albums/" + id.replace("spotify:album:", ""));
        }
        else // Element is playlist
        {
            QString playlist = id.right(id.length() - id.lastIndexOf(':') - 1);

            url = QUrl("https://api.spotify.com/v1/playlists/" + playlist + "/images");
        }

        connect(requestor, qOverload<int, QNetworkReply::NetworkError, QByteArray>(&O2Requestor::finished), this, &Spotify::fetchedIcon);

        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, O2_MIME_TYPE_JSON);
        element->setIconRequestId(requestor->get(request));

        qDebug() << "   Request sent.";
    }
    else
    {
        if (!iconFetchQueue.contains(element)) iconFetchQueue.enqueue(element);
    }
}

/**
 * @brief Fetch all the icons in the queue
 */
void Spotify::fetchQueuedIcons()
{
    qDebug() << "SPOTIFY: Fetching queued element icons ...";

    while (!iconFetchQueue.isEmpty())
    {
        auto a = iconFetchQueue.dequeue();
        fetchIcon(a);
    }
}

/**
 * @brief Called when a GET request for an icon was received
 * @param id ID of the GET request
 * @param error NetworkError of request
 * @param data Contains the received data in json form
 */
void Spotify::fetchedIcon(int id, QNetworkReply::NetworkError error, QByteArray data)
{
    if (m_elements.size() < 1) return;

    qDebug() << "Fetched Spotify icon:" << id;
    qDebug() << "   Error:" << error;

    for (auto e : m_elements)
    {
        if (e && (e->iconRequestId() == id))
        {
            const auto  doc = QJsonDocument::fromJson(data);
            QJsonObject image;

            if (doc.isArray()) // Array of Images
            {
                image = doc.array()[0].toObject();
            }
            else // Object containing an Array of Images
            {
                image = doc.object().value("images").toArray()[0].toObject();
            }

            e->setIcon(image.value("url").toString());

            qDebug() << "   Icon set successfully.";

            return;
        }
    }

    qDebug() << "SPOTIFY: Could not find element with iconRequestId" << id;
}

/**
 * @brief Add spotify elements from audio project
 * @param elements List of pointers to SpotifyElements
 */
void Spotify::addElements(QList<SpotifyElement *>elements)
{
    for (auto e : elements)
    {
        if (!m_elements.contains(e))
        {
            m_elements.append(e);
        }
    }
}
