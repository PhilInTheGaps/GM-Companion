#pragma once

#include "../metadata/metadatareader.h"
#include "../project/audioelement.h"
#include "audioplayer.h"
#include "filesystem/file.h"
#include <QAudioOutput>
#include <QBuffer>
#include <QMediaPlayer>
#include <QTemporaryDir>

class RadioPlayer : public AudioPlayer
{
    Q_OBJECT
public:
    RadioPlayer(MetaDataReader &metaDataReader, QObject *parent = nullptr);

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
    QMediaPlayer m_mediaPlayer;
    QAudioOutput m_audioOutput;
    QObject *m_fileRequestContext = nullptr;

    AudioElement *m_currentElement = nullptr;

    QBuffer m_mediaBuffer;
    QTemporaryDir m_tempDir;
    QString m_fileName;

signals:
    void startedPlaying();
    void metaDataChanged(const QMediaMetaData &metaData);

private slots:
    void onMetaDataChanged();
    void onFileReceived(Files::FileDataResult *result);
    void onMediaPlayerPlaybackStateChanged(QMediaPlayer::PlaybackState newState);
    void onMediaPlayerMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onMediaPlayerErrorOccurred(QMediaPlayer::Error error, const QString &errorString);
};
