#ifndef SPOTIFY_H
#define SPOTIFY_H

#include <QQueue>
#include <QBuffer>
#include <QTimer>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include "lib/o2/src/o2spotify.h"
#include "src/settings/settingsmanager.h"
#include "audioplayer.h"
#include "../audioelement.h"
#include "../metadatareader.h"

struct SpotifyTrack
{
    QString title;
    QString id;
};

class Spotify : public AudioPlayer
{
    Q_OBJECT

public:
    explicit Spotify(FileManager *fManager, MetaDataReader *mDReader);
    ~Spotify();

    void grant();
    bool isGranted() const { return m_spotify->linked(); }
    bool isWaitingForAuth() const { return m_waitingForAuth; }
    QUrl authUrl() const { return m_authUrl; }

    void play(QString id, int offset = -1, bool playOnce = false);
    void play();
    void pause() { stop(); }
    void stop();
    void pausePlay();

    void setIndex(int index) { /*play(m_currentId, index);*/ }
    int index() const { return m_currentIndex; }
    void next();
    void again();

    void setVolume(int volume);
    bool isPlaying() const { return m_isPlaying; }
    void getPlaylistTracks(QString id);

//    void fetchIcons(AudioScenario *scenario);
//    QStringList songNames() const { return m_trackList; }

private:
    O2Spotify *m_spotify = nullptr;
    MetaDataReader *metaDataReader = nullptr;
    MusicElement *currentElement = nullptr;

    SettingsManager m_sManager;
    QTimer *m_timer;
    QTimer *m_periodicTimer;
    QNetworkAccessManager *m_manager;

    QQueue<MusicElement*>iconFetchQueue;
//    void fetchIcon(SpotifyElement *element);
//    void fetchQueuedIcons();

    QList<MusicElement*> m_elements;

    QString m_currentId;
    bool m_isPlaying = false;
    bool m_playOnce = false;
    int m_volume;
    int m_currentIndex = 0;
    QStringList m_trackList;
    QStringList m_trackIdList;
    QString m_currentSongName;
    bool m_waitingForAuth = false;
    QUrl m_authUrl;

    void forceCurrentMachine();
    void openSpotify();

    void getCurrentSong();
    void startTimer(int interval = -1);
    void put(QUrl url, QString params = "");

signals:
    void iconChanged(int index, QString url);
    void authorize(QUrl url);
    void authorized();
    void songNamesChanged();
    void startedPlaying();
    void songEnded();
    void receivedPlaylistTracks(QList<SpotifyTrack> tracks, QString id);

private slots:
    void authorizeReady(QUrl url) { emit authorize(url); }
    void granted();
    void gotPlaylistInfo(int id, QNetworkReply::NetworkError error, QByteArray data);
    void fetchedIcon(int id, QNetworkReply::NetworkError error, QByteArray data);
};


#endif // SPOTIFY_H
