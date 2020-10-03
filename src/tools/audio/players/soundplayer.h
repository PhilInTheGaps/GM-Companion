#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <QBuffer>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTemporaryDir>

#include "../project/audioelement.h"
#include "audioplayer.h"
#include "discordplayer.h"
#include <qytlib/videos/videoclient.h>

class SoundPlayer : public AudioPlayer
{
    Q_OBJECT

public:
    SoundPlayer(AudioElement *element, int volume, QNetworkAccessManager *networkManager,
                DiscordPlayer *discordPlayer, QObject *parent = nullptr);
    ~SoundPlayer();

    AudioElement *element() const { return m_element; }
    void loadMedia(AudioFile *file);
    QString fileName() const { return m_fileName; }

public slots:
    void play();
    void pause() { m_mediaPlayer->pause(); }
    void stop();
    void stopElement(const QString& element);
    void setLogarithmicVolume(int volume) { m_mediaPlayer->setVolume(volume); }
    void setLinearVolume(int volume) { }
    void again() {}
    void next();
    void setIndex(int index) {}

private:
    AudioElement *m_element = nullptr;
    QMediaPlayer *m_mediaPlayer = nullptr;
    DiscordPlayer *m_discordPlayer = nullptr;
    YouTube::Videos::VideoClient *m_videoClient = nullptr;
    YouTube::Videos::Streams::StreamManifest *m_streamManifest = nullptr;

    QList<AudioFile*> m_playlist;
    int m_playlistIndex = 0;
    int m_fileRequestId = -1;
    int m_youtubeRequestId = -1;

    QBuffer m_mediaBuffer;
    QTemporaryDir m_tempDir;
    QString m_fileName;

    void applyShuffleMode();

private slots:
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onMediaPlayerError(QMediaPlayer::Error error);
    void onFileReceived(int requestId, const QByteArray& data);
    void onStreamManifestReceived();

signals:
    void playerStopped(SoundPlayer *player);
};


class SoundPlayerController : public AudioPlayer
{
    Q_OBJECT
public:
    SoundPlayerController(DiscordPlayer *discordPlayer, QObject *parent = nullptr)
        : AudioPlayer(parent), m_discordPlayer(discordPlayer),
          m_networkManager(new QNetworkAccessManager(this))
    {
        m_networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
    }

    void play(AudioElement* elements);
    void stop(QString element);

public slots:
    void play() { }
    void pause() { }
    void stop() { emit stopAll(); }
    void setLogarithmicVolume(int volume);
    void setLinearVolume(int volume) {}
    void next() { }
    void again() { }
    void setIndex(int index) { }

private:
    DiscordPlayer *m_discordPlayer = nullptr;
    QNetworkAccessManager *m_networkManager = nullptr;
    QList<SoundPlayer*> m_players;
    int m_volume = 0;

    QList<AudioElement *> elements() const;
    bool isSoundPlaying(AudioElement* elements);

private slots:
    void onPlayerStopped(SoundPlayer *player);

signals:
    void setVolume(int volume);
    void stopElement(QString element);
    void stopAll();
    void soundsChanged(QList<AudioElement*>);
};

#endif // SOUNDPLAYER_H
