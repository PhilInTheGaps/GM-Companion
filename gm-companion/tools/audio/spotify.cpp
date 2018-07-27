#include "spotify.h"
#include <QDesktopServices>
#include <QJsonDocument>
#include <QDebug>

Spotify::Spotify(QObject *parent) : QObject(parent)
{
    qDebug() << "Starting Auth ...";

    auto replyHandler = new QOAuthHttpServerReplyHandler(8080, this);
    m_spotify.setReplyHandler(replyHandler);

    m_spotify.setAuthorizationUrl(QUrl("https://accounts.spotify.com/authorize"));
    m_spotify.setAccessTokenUrl(QUrl("https://accounts.spotify.com/api/token"));

    // Client ID
    m_spotify.setClientIdentifier(m_sManager.getSetting(Setting::spotifyID));
    m_spotify.setClientIdentifierSharedKey(m_sManager.getSetting(Setting::spotifySecret));

    // Scopes
    m_spotify.setScope("user-library-read playlist-read-private streaming user-modify-playback-state user-read-currently-playing "
                       "user-read-playback-state");

    // Signals
    connect(&m_spotify, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, &QDesktopServices::openUrl);
    connect(&m_spotify, &QOAuth2AuthorizationCodeFlow::statusChanged,        this, &Spotify::authStatusChanged);
    connect(&m_spotify, &QOAuth2AuthorizationCodeFlow::granted,              this, &Spotify::granted);
}

void Spotify::authStatusChanged(QAbstractOAuth::Status status)
{
    QString s;

    if (status == QAbstractOAuth::Status::Granted) s = "granted";

    if (status == QAbstractOAuth::Status::TemporaryCredentialsReceived) {
        s = "temp credentials";
    }

    qDebug() << "Status Changed:" << s;
}

void Spotify::granted()
{
    m_isGranted = true;

    switch (m_afterGranted)
    {
    case 1:
        play(m_tempId);
        break;

    default:
        break;
    }
}

void Spotify::play(QString id)
{
    if (m_isGranted)
    {
        qDebug() << "Playing:" << id;

        QJsonObject jo;
        jo.insert("context_uri", id);

        setVolume(m_volume);
        m_spotify.put(QUrl("https://api.spotify.com/v1/me/player/play"), QJsonDocument(jo).toJson());

        m_isPlaying = true;
    }
    else
    {
        m_tempId       = id;
        m_afterGranted = 1;
        m_spotify.grant();
    }
}

void Spotify::play()
{
    qDebug() << "Playing...";

    m_spotify.put(QUrl("https://api.spotify.com/v1/me/player/play"));

    m_isPlaying = true;
}

void Spotify::stop()
{
    m_spotify.put(QUrl("https://api.spotify.com/v1/me/player/pause"));

    m_isPlaying = false;
}

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

void Spotify::setIndex(int index)
{
}

void Spotify::next()
{
    qDebug() << "Switching to next song ...";

    auto reply = m_spotify.post(QUrl("https://api.spotify.com/v1/me/player/next"));

    connect(reply, &QNetworkReply::finished, [ = ]() {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Error:" << reply->errorString();
            return;
        }
        const auto data = reply->readAll();
        qDebug() << data;
        reply->deleteLater();
    });
}

void Spotify::again()
{
    qDebug() << "Playing track again ...";

    m_spotify.put(QUrl("https://api.spotify.com/v1/me/player/seek?position_ms=" + QString::number(1)));
}

void Spotify::setVolume(int volume)
{
    qDebug() << "Setting Volume:" << volume << "...";

    m_spotify.put(QUrl("https://api.spotify.com/v1/me/player/volume?volume_percent=" + QString::number(volume)));

    m_volume = volume;
}

void Spotify::fetchIcon(QString id, int index)
{
    if (m_isGranted)
    {
        qDebug() << "Fetching thumbnail ..." << id;

        QNetworkReply *reply;

        if (id.contains("album:"))
        {
            reply = m_spotify.get("https://api.spotify.com/v1/albums/" + id.replace("spotify:album:", ""));

            connect(reply, &QNetworkReply::finished, [ = ]() {
                if (reply->error() != QNetworkReply::NoError) {
                    qDebug() << "Error:" << reply->errorString();
                    return;
                }
                const auto data     = reply->readAll();
                const auto document = QJsonDocument::fromJson(data);
                const auto root     = document.object();
                const auto images   = root.value("images").toArray();
                const auto image    = images.at(0).toObject();

                QString url = image.value("url").toString();
                qDebug() << url;
                reply->deleteLater();
                emit iconChanged(index, url);
            });
        }
        else
        {
            QString user = id.replace("spotify:user:", "");
            user = user.left(user.indexOf(":"));
            qDebug() << user;

            QString replaceString = "spotify:user:";
            QString playlist      = id.replace(replaceString, "").replace(user, "").replace(":playlist:", "");
            qDebug() << playlist;

            reply = m_spotify.get("https://api.spotify.com/v1/users/" + user + "/playlists/" + playlist + "/images");

            connect(reply, &QNetworkReply::finished, [ = ]() {
                if (reply->error() != QNetworkReply::NoError) {
                    qDebug() << "Error:" << reply->errorString();
                    return;
                }
                const auto data     = reply->readAll();
                const auto document = QJsonDocument::fromJson(data);
                const auto root     = document.array();
                const auto image    = root.at(0).toObject();

                QString url = image.value("url").toString();
                qDebug() << url;
                reply->deleteLater();
                emit iconChanged(index, url);
            });
        }
    }
    else
    {
        //        m_tempId       = id;
        //        m_afterGranted = 2;
        //        grant();
    }
}
