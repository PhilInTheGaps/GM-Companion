#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "../audioelement.h"
#include "audioplayer.h"
#include "spotifyplayer.h"
#include "settings/settingsmanager.h"
#include "youtube.h"

class MusicPlayer : public AudioPlayer
{
    Q_OBJECT
public:
    explicit MusicPlayer(FileManager *fManager, SpotifyPlayer *spotifyPlayer);
    ~MusicPlayer();

    void play(MusicElement *element);
    void play();
    void pause();
    void stop();
    void setVolume(float volume);
    void setVolume(int volume);
    void next();
    void again();
    void setIndex(int index);

    QStringList songNames() const { return m_songNames; }
    int index() const { return m_playlistIndex; }

private:
    SpotifyPlayer *spotifyPlayer;
    QMediaPlayer *mediaPlayer;
    MusicElement *currentElement;
    SettingsManager sManager;
    YouTube youtube;

    QList<AudioFile*> m_playlist;
    int m_playlistIndex;
    int m_playerType;
    int m_youtubeRequestId;
    void loadMedia(AudioFile *file);
    void loadSongNames(bool initial = false, bool reloadYt = false);
    void applyShuffleMode(bool keepIndex = false, QString url = "");

    QStringList m_songNames;
    bool m_waitingForUrls;

signals:
    void startedPlaying();
    void metaDataChanged(QMediaPlayer *mediaPlayer, QString elementIcon);
    void currentIndexChanged();
    void songNamesChanged();

//public slots:
//    void onSongPathsChanged(QList<QUrl> urls);

private slots:
    void onMetaDataChanged();
    void onMediaPlayerStateChanged();
    void onMediaPlayerBufferStatusChanged();
    void onMediaPlayerMediaStatusChanged();
    void onMediaPlayerError(QMediaPlayer::Error error);
    void onMediaPlayerMediaChanged();
    void onSpotifySongEnded();
    void onSpotifyReceivedPlaylistTracks(QList<SpotifyTrack>tracks, QString playlistId);
    void onYtReceivedVideoMediaStreamInfos(MediaStreamInfoSet *infos, int reqId);
    void onYtReceivedVideo(Video video, int reqId);
};

#endif // MUSICPLAYER_H
