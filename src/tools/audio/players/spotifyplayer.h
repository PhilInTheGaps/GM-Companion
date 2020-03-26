#ifndef SPOTIFYPLAYER_H
#define SPOTIFYPLAYER_H

#include <QQueue>
#include <QBuffer>
#include <QTimer>
#include <QNetworkAccessManager>

#include "audioplayer.h"
#include "../audioelement.h"
#include "../metadatareader.h"

struct SpotifyTrack
{
    QString title;
    QString id;
};

class SpotifyPlayer : public AudioPlayer
{
    Q_OBJECT

public:
    explicit SpotifyPlayer(FileManager *fManager, MetaDataReader *mDReader);
    ~SpotifyPlayer();

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

private:
    MetaDataReader *metaDataReader = nullptr;
    QNetworkAccessManager *m_networkManager = nullptr;
    QTimer *m_timer;
    QTimer *m_periodicTimer;

    QMap<QString, int> m_requestIdMap;

    bool m_isPlaying = false;
    bool m_playOnce = false;
    int m_volume;
    int m_currentIndex = 0;

    QStringList m_trackList;
    QStringList m_trackIdList;

    void getCurrentSong();
    void startTimer(int interval = -1);

signals:
    void songNamesChanged();
    void startedPlaying();
    void songEnded();
    void receivedPlaylistTracks(QList<SpotifyTrack> tracks, QString id);
    void receivedElementIcon(AudioElement *element);

private slots:
    void gotPlaylistInfo(int id, QNetworkReply::NetworkError error, QByteArray data);
};


#endif // SPOTIFYPLAYER_H
