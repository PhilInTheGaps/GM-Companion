#ifndef SPOTIFYPLAYER_H
#define SPOTIFYPLAYER_H

#include <QQueue>
#include <QBuffer>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "audioplayer.h"
#include "discordplayer.h"
#include "../project/audioelement.h"
#include "../metadata/metadatareader.h"

struct SpotifyTrack
{
    QString title;
    QString uri;
};

class SpotifyPlayer : public AudioPlayer
{
    Q_OBJECT

public:
    SpotifyPlayer(MetaDataReader *mDReader, DiscordPlayer *discordPlayer, QObject *parent = nullptr);
    ~SpotifyPlayer();

    void play(const QString& id, int offset = -1, bool playOnce = false);
    void play();
    void pause() { stop(); }
    void stop();
    void pausePlay();

    void setIndex(int index) { /*play(m_currentId, index);*/ }
    int index() const { return m_currentIndex; }
    void next();
    void again();

    void setLogarithmicVolume(int volume) { }
    void setLinearVolume(int volume);
    bool isPlaying() const { return m_isPlaying; }
    void getPlaylistTracks(const QString& uri);

private:
    MetaDataReader *metaDataReader = nullptr;
    QNetworkAccessManager *m_networkManager = nullptr;
    DiscordPlayer *m_discordPlayer = nullptr;
    QTimer *m_timer;
    QTimer *m_periodicTimer;

    QMap<QString, int> m_requestIdMap;

    bool m_isPlaying = false;
    bool m_playOnce = false;
    int m_volume = 0;
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
    void gotPlaylistInfo(int id, QNetworkReply::NetworkError error, const QByteArray& data);
};


#endif // SPOTIFYPLAYER_H
