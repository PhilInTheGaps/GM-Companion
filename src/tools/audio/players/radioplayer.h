#pragma once

#include "../metadata/metadatareader.h"
#include "../project/audioelement.h"
#include "audioplayer.h"
#include <QAudioOutput>
#include <QBuffer>
#include <QMediaPlayer>
#include <QNetworkAccessManager>
#include <QTemporaryDir>

namespace Files
{
class FileDataResult;
}

class RadioPlayer : public AudioPlayer
{
    Q_OBJECT
public:
    RadioPlayer(QNetworkAccessManager &networkManager, MetaDataReader &metaDataReader, QObject *parent = nullptr);

public slots:
    void play(AudioElement *element);
    void play() override;
    void pause() override;
    void stop() override;
    void setVolume(int linear, int logarithmic) override;
    void again() override
    {
    }
    void next() override
    {
    }

private:
    QNetworkAccessManager &m_networkManager;
    QMediaPlayer m_mediaPlayer;
    QAudioOutput m_audioOutput;
    QObject *m_fileRequestContext = nullptr;

    AudioElement *m_currentElement = nullptr;

    QBuffer m_mediaBuffer;
    QTemporaryDir m_tempDir;
    QString m_fileName;

    void play(const AudioFile *audioFile);

    static auto isPlaylist(const QString &file) -> bool;

    void playAudio(const QByteArray &data);
    void playPlaylist(const QByteArray &data);

signals:
    void startedPlaying();
    void metaDataChanged(const QMediaMetaData &metaData);

private slots:
    void onMetaDataChanged();
    void onFileReceived(std::shared_ptr<Files::FileDataResult> result);
    void onMediaPlayerPlaybackStateChanged(QMediaPlayer::PlaybackState newState);
    void onMediaPlayerMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onMediaPlayerErrorOccurred(QMediaPlayer::Error error, const QString &errorString);
};
