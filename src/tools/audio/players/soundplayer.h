#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <QBuffer>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTemporaryDir>

#include "../audioelement.h"
#include "audioplayer.h"
#include "youtube.h"

class SoundPlayer : public AudioPlayer
{
    Q_OBJECT

public:
    SoundPlayer(AudioElement *element, int volume, QObject *parent = nullptr);
    ~SoundPlayer();

    AudioElement *element() const { return m_element; }
    void loadMedia(AudioFile *file);

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
    YouTube youtube;

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
    void onReceivedAudioStreamInfo(MediaStreamInfoSet *infos, int requestId);
    void onFileReceived(int requestId, const QByteArray& data);

signals:
    void playerStopped(SoundPlayer *player);
};


class SoundPlayerController : public AudioPlayer
{
    Q_OBJECT
public:
    SoundPlayerController(QObject *parent = nullptr) : AudioPlayer(parent) {}

    void play(AudioElement* elements);
    void stop(QString element) { emit stopElement(element); }

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
