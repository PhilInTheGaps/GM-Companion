#ifndef SPOTIFY_H
#define SPOTIFY_H

#include <QObject>
#include <QQueue>
#include <QBuffer>
#include <QTimer>

#include "lib/o2/src/o2spotify.h"
#include "src/settings/settingsmanager.h"

struct Playlist
{
    QString id;
    int     index;
};

class Spotify : public QObject {
    Q_OBJECT

public:
    explicit Spotify(QObject *parent = nullptr);
    ~Spotify() {}

    void grant() { m_spotify->link(); }
    bool isGranted() const { return m_spotify->linked(); }

    void play(QString id, int offset = 0);
    void play();
    void stop();
    void pausePlay();

    void setIndex(int index);
    int getIndex() const { return m_currentIndex; }
    void next();
    void again();

    void setVolume(int volume);
    bool isPlaying() const { return m_isPlaying; }

    void fetchIcon(QString id, int index);

private:
    O2Spotify *m_spotify;
    SettingsManager m_sManager;
    QTimer *m_timer;

    QQueue<Playlist>iconFetchQueue;

    QString m_tempId;
    QString m_currentId;
    bool m_isPlaying = false;
    int m_volume;
    int m_afterGranted = 0;
    int m_currentIndex = 0;
    QStringList m_trackList;
    QStringList m_trackIdList;
    QString m_currentSongName;

    void fetchQueuedIcons();
    void getCurrentSong();
    void getCurrentPlaylist();
    void put(QUrl url, QString params = "");

signals:
    void iconChanged(int index, QString url);
    void authorize(QUrl url);
    void authorized();
    void currentSongChanged(QString title, QString artist, QString album, QString image);
    void currentPlaylistChanged(QStringList songs);

private slots:
    void authorizeReady(QUrl url) { emit authorize(url); }
    void granted();
};


#endif // SPOTIFY_H
