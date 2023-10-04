#pragma once

#include "audioplayer.h"
#include "filesystem/results/filedataresult.h"
#include <QAudioOutput>
#include <QBuffer>
#include <QMediaPlayer>
#include <QQmlListProperty>
#include <QTemporaryDir>
#include <QtQml/qqmlregistration.h>
#include <memory>

class AudioElement;
class AudioFile;

class SoundPlayer : public AudioPlayer
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

public:
    SoundPlayer(AudioElement *element, int volume, QObject *parent = nullptr);

    [[nodiscard]] auto element() const -> AudioElement *
    {
        return m_element;
    }
    [[nodiscard]] auto fileName() const -> QString
    {
        return m_fileName;
    }
    void loadMedia(const AudioFile *file);

public slots:
    void play() override;
    void pause() override;
    void stop() override;
    void stopElement(const QString &element);
    void setVolume(int linear, int logarithmic) override;
    void again() override
    {
    }
    void next() override;

private:
    QPointer<AudioElement> m_element = nullptr;
    QMediaPlayer m_mediaPlayer;
    QAudioOutput m_audioOutput;
    std::unique_ptr<QObject> m_fileRequestContext = nullptr;

    QList<AudioFile *> m_playlist;
    int m_playlistIndex = 0;
    int m_youtubeRequestId = -1;

    std::unique_ptr<QBuffer> m_mediaBuffer = nullptr;
    QTemporaryDir m_tempDir;
    QString m_fileName;

    void applyShuffleMode();

private slots:
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onMediaPlayerErrorOccurred(QMediaPlayer::Error error, const QString &errorString);
    void onFileReceived(std::shared_ptr<Files::FileDataResult> result);

signals:
    void playerStopped(SoundPlayer *player);
};
