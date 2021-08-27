#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTemporaryDir>

#include "../project/audioelement.h"
#include "audioplayer.h"
#include "spotifyplayer.h"
#include "discordplayer.h"
#include "filesystem/file.h"
#include <qytlib/videos/videoclient.h>

class MusicPlayer : public AudioPlayer
{
    Q_OBJECT
public:
    explicit MusicPlayer(MetaDataReader *metaDataReader, SpotifyPlayer *spotifyPlayer,
                         DiscordPlayer *discordPlayer, QObject *parent = nullptr);

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
    DiscordPlayer *discordPlayer = nullptr;
    YouTube::Videos::VideoClient *videoClient = nullptr;
    YouTube::Videos::Streams::StreamManifest *m_streamManifest = nullptr;
    QObject *m_fileRequestContext = nullptr;

    QList<AudioFile*> m_playlist;
    int m_playlistIndex = 0;
    int m_playerType = -1;

    QBuffer m_mediaBuffer;
    QTemporaryDir m_tempDir;
    QString m_fileName;

    void loadMedia(AudioFile *file);
    void loadSongNames(bool initial = false, bool reloadYt = false);
    void applyShuffleMode(bool keepIndex = false, const QString& url = "");
    void startPlaying();

    QStringList m_songNames;
    int m_waitingForUrls = 0;

    void connectPlaybackStateSignals();
    void connectMetaDataSignals(MetaDataReader *metaDataReader);
    void connectSpotifySignals();

private slots:
    void onMediaPlayerStateChanged();
    void onMediaPlayerBufferStatusChanged();
    void onMediaPlayerMediaStatusChanged();
    void onMediaPlayerError(QMediaPlayer::Error error);
    void onMediaPlayerMediaChanged();
    void onFileReceived(Files::FileDataResult *result);
    void onSpotifySongEnded();
    void onSpotifyReceivedPlaylistTracks(QList<SpotifyTrack>tracks, const QString& playlistId);
    void onVideoMetadataReceived();
    void onStreamManifestReceived();

signals:
    void startedPlaying();
    void clearMetaData();
    void metaDataChanged(QMediaPlayer *mediaPlayer);
    void metaDataChanged(const QString& key, const QVariant& value);
    void metaDataChanged(const QByteArray& data);
    void currentIndexChanged();
    void songNamesChanged();
};

#endif // MUSICPLAYER_H
