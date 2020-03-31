#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "../audioelement.h"
#include "audioplayer.h"
#include "spotifyplayer.h"
#include "youtube.h"

class MusicPlayer : public AudioPlayer
{
    Q_OBJECT
public:
    MusicPlayer(SpotifyPlayer *spotifyPlayer, QObject *parent = nullptr);
    ~MusicPlayer();

    void play(AudioElement *element);

    QStringList songNames() const { return m_songNames; }
    int index() const { return m_playlistIndex; }

public slots:
    void play();
    void pause();
    void stop();
    void setLogarithmicVolume(int volume);
    void setLinearVolume(int volume);
    void next();
    void again();
    void setIndex(int index);

private:
    SpotifyPlayer *spotifyPlayer;
    QMediaPlayer *mediaPlayer;
    AudioElement *currentElement;
    YouTube youtube;

    QList<AudioFile*> m_playlist;
    int m_playlistIndex;
    int m_playerType;
    int m_youtubeRequestId;
    int m_fileRequestId;

    QByteArray m_mediaData;
    QBuffer *m_mediaBuffer = nullptr;

    void loadMedia(AudioFile *file);
    void loadSongNames(bool initial = false, bool reloadYt = false);
    void applyShuffleMode(bool keepIndex = false, QString url = "");
    void startPlaying();

    QStringList m_songNames;
    int m_waitingForUrls = 0;

private slots:
    void onMetaDataChanged();
    void onMediaPlayerStateChanged();
    void onMediaPlayerBufferStatusChanged();
    void onMediaPlayerMediaStatusChanged();
    void onMediaPlayerError(QMediaPlayer::Error error);
    void onMediaPlayerMediaChanged();
    void onFileReceived(int id, QByteArray data);
    void onSpotifySongEnded();
    void onSpotifyReceivedPlaylistTracks(QList<SpotifyTrack>tracks, QString playlistId);
    void onYtReceivedVideoMediaStreamInfos(MediaStreamInfoSet *infos, int reqId);
    void onYtReceivedVideo(Video video, int reqId);

signals:
    void startedPlaying();
    void metaDataChanged(QMediaPlayer *mediaPlayer, QPixmap elementIcon);
    void currentIndexChanged();
    void songNamesChanged();
};

#endif // MUSICPLAYER_H
