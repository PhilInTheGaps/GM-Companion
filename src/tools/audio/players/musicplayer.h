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
    explicit MusicPlayer(SpotifyPlayer *spotifyPlayer, QObject *parent = nullptr);
    ~MusicPlayer() override;

    void play(AudioElement *element);

    QStringList songNames() const { return m_songNames; }
    int index() const { return m_playlistIndex; }

public slots:
    void play() override;
    void pause() override;
    void stop() override;
    void setLogarithmicVolume(int volume) override;
    void setLinearVolume(int volume) override;
    void next() override;
    void again() override;
    void setIndex(int index) override;

private:
    SpotifyPlayer *spotifyPlayer = nullptr;
    QMediaPlayer *mediaPlayer = nullptr;
    AudioElement *currentElement = nullptr;
    YouTube youtube;

    QList<AudioFile*> m_playlist;
    int m_playlistIndex = 0;
    int m_playerType = -1;
    int m_youtubeRequestId = -1;
    int m_fileRequestId = -1;

    QByteArray m_mediaData;
    QBuffer *m_mediaBuffer = nullptr;

    void loadMedia(AudioFile *file);
    void loadSongNames(bool initial = false, bool reloadYt = false);
    void applyShuffleMode(bool keepIndex = false, const QString& url = "");
    void startPlaying();

    QStringList m_songNames;
    int m_waitingForUrls = 0;

private slots:
    void onMediaPlayerStateChanged();
    void onMediaPlayerBufferStatusChanged();
    void onMediaPlayerMediaStatusChanged();
    void onMediaPlayerError(QMediaPlayer::Error error);
    void onMediaPlayerMediaChanged();
    void onFileReceived(int id, const QByteArray& data);
    void onSpotifySongEnded();
    void onSpotifyReceivedPlaylistTracks(QList<SpotifyTrack>tracks, const QString& playlistId);
    void onYtReceivedVideoMediaStreamInfos(MediaStreamInfoSet *infos, int reqId);
    void onYtReceivedVideo(const Video& video, int reqId);

signals:
    void startedPlaying();
    void clearMetaData();
    void metaDataChanged(QMediaPlayer *mediaPlayer);
    void metaDataChanged(const QString& key, const QVariant& value);
    void currentIndexChanged();
    void songNamesChanged();
};

#endif // MUSICPLAYER_H
