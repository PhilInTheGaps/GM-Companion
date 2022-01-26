#ifndef SPOTIFYPLAYER_H
#define SPOTIFYPLAYER_H

#include <QQueue>
#include <QBuffer>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "rest/restnetworkreply.h"
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
    SpotifyPlayer(MetaDataReader *mDReader, DiscordPlayer *discordPlayer,
                  QNetworkAccessManager *networkManager, QObject *parent = nullptr);
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

    bool m_isPlaying = false;
    bool m_playOnce = false;
    int m_volume = 0;
    int m_currentIndex = 0;

    QStringList m_trackList;
    QStringList m_trackIdList;

    void getCurrentSong();
    void startTimer(int interval = -1);

    void gotPlaylistInfo(RestNetworkReply *reply);

signals:
    void songNamesChanged();
    void startedPlaying();
    void songEnded();
    void receivedPlaylistTracks(QList<SpotifyTrack> tracks, QString id);
    void receivedElementIcon(AudioElement *element);

};


#endif // SPOTIFYPLAYER_H
