#pragma once

#include "../project/audioelement.h"
#include "audioplayer.h"
#include "filesystem/results/filedataresult.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QAudioOutput>
#include <QBuffer>
#include <QMediaPlayer>
#include <QQmlListProperty>
#include <QTemporaryDir>
#include <QtQml/qqmlregistration.h>

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
    AudioElement *m_element = nullptr;
    QMediaPlayer m_mediaPlayer;
    QAudioOutput m_audioOutput;
    QObject *m_fileRequestContext = nullptr;

    QList<AudioFile *> m_playlist;
    int m_playlistIndex = 0;
    int m_youtubeRequestId = -1;

    QBuffer m_mediaBuffer;
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

class SoundPlayerController : public AudioPlayer
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    READ_LIST_PROPERTY(AudioElement, activeElements)

public:
    explicit SoundPlayerController(QObject *parent = nullptr);

    void play(AudioElement *elements);
    void stop(const QString &element);

public slots:
    void play() override
    {
    }
    void pause() override
    {
    }
    void stop() override
    {
        emit stopAll();
    }
    void setVolume(int linear, int logarithmic) override;
    void next() override
    {
    }
    void again() override
    {
    }

private:
    QList<SoundPlayer *> m_players;
    int m_volume = 0;

    [[nodiscard]] auto elements() const -> QList<AudioElement *>;
    [[nodiscard]] auto isSoundPlaying(AudioElement *elements) const -> bool;
    void updateActiveElements();

private slots:
    void onPlayerStopped(SoundPlayer *player);
    void onSoundsChanged(const QList<AudioElement *> &sounds);

signals:
    void setPlayerVolume(int linear, int logarithmic);
    void stopElement(QString element);
    void stopAll();
    void soundsChanged(QList<AudioElement *>);
};
