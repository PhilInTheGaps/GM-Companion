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

class Spotify : public AudioPlayer
{
    Q_OBJECT

public:
    explicit Spotify(FileManager *fManager, MetaDataReader *mDReader);
    ~Spotify() { stop(); }

    void grant();
    bool isGranted() const { return m_spotify->linked(); }

    void play(QString id, int offset = -1);
    void play(SpotifyElement *element) { play(element->id(), 0); currentElement = element; }
    void play();
    void pause() { stop(); }
    void stop();
    void pausePlay();

    void setIndex(int index) { play(m_currentId, index); }
    int index() const { return m_currentIndex; }
    void next();
    void again();

    void setVolume(int volume);
    bool isPlaying() const { return m_isPlaying; }

    void fetchIcons(AudioScenario *scenario);
    QStringList songNames() const { return m_trackList; }

    void addElements(QList<SpotifyElement*> elements);

private:
    O2Spotify *m_spotify = nullptr;
    MetaDataReader *metaDataReader = nullptr;
    SpotifyElement *currentElement = nullptr;

    SettingsManager m_sManager;
    QTimer *m_timer;
    QNetworkAccessManager *m_manager;

    QQueue<SpotifyElement*>iconFetchQueue;
    void fetchIcon(SpotifyElement *element);
    void fetchQueuedIcons();

    QList<SpotifyElement*> m_elements;

    QString m_currentId;
    bool m_isPlaying = false;
    int m_volume;
    int m_currentIndex = 0;
    QStringList m_trackList;
    QStringList m_trackIdList;
    QString m_currentSongName;

    void getCurrentSong();
    void getCurrentPlaylist();
    void put(QUrl url, QString params = "");

signals:
    void iconChanged(int index, QString url);
    void authorize(QUrl url);
    void authorized();
    void songNamesChanged();
    void startedPlaying();

private slots:
    void authorizeReady(QUrl url) { emit authorize(url); }
    void granted();
    void gotCurrentPlaylist(int id, QNetworkReply::NetworkError error, QByteArray data);
    void fetchedIcon(int id, QNetworkReply::NetworkError error, QByteArray data);
};


#endif // SPOTIFY_H
