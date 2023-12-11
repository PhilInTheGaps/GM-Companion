#pragma once

#include "../playlist/resolvingaudioplaylist.h"
#include "../project/audioelement.h"
#include "audioplayer.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QAudioOutput>
#include <QBuffer>
#include <QFuture>
#include <QMediaMetaData>
#include <QMediaPlayer>
#include <QNetworkAccessManager>
#include <QQmlEngine>
#include <memory>

namespace Files
{
class FileDataResult;
}

class BufferedAudioPlayer : public AudioPlayer
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("")

    Q_PROPERTY(QQmlListProperty<AudioFile> playlist READ playlistQml NOTIFY playlistChanged FINAL)
    AUTO_PROPERTY_VAL2(qsizetype, playlistIndex, 0);

public:
    explicit BufferedAudioPlayer(const QString &settingsId, QNetworkAccessManager *networkManager, QObject *parent);

    [[nodiscard]] auto playlistQml() -> QQmlListProperty<AudioFile>;
    [[nodiscard]] auto element() const -> QPointer<AudioElement>;

    void setIndex(qsizetype index);

public slots:
    virtual void play(AudioElement *element);
    void play() override;
    void pause() override;
    void stop() override;
    void setVolume(int linear, int logarithmic) override;
    void again() override;
    void next() override;

signals:
    void playlistChanged();
    void metaDataChanged(const QMediaMetaData &data);
    void currentFileChanged(const QString &path, const QByteArray &data);

protected:
    void play(const QByteArray &data);
    auto loadPlaylist() -> QFuture<void>;
    virtual void handleUnsupportedMediaSource(const AudioFile &file);
    void onFileReceived(const Files::FileDataResult &result);

    [[nodiscard]] auto fileSource() const -> AudioFile::Source;
    void setPlaylist(std::unique_ptr<ResolvingAudioPlaylist> playlist);

    void loadMedia(AudioFile &file);
    void loadLocalFile(const AudioFile &file);
    void loadWebFile(const AudioFile &file);
    void loadWebFile(const QString &url);
    void loadYouTubeFile(AudioFile &file);

private slots:
    void onMediaPlayerPlaybackStateChanged(QMediaPlayer::PlaybackState newState);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void onMediaPlayerErrorOccurred(QMediaPlayer::Error error, const QString &errorString);

private:
    void startPlaying();
    void applyShuffleMode();

    QPointer<QNetworkAccessManager> m_networkManager = nullptr;

    QString m_settingsId;
    QMediaPlayer m_mediaPlayer;
    QAudioOutput m_audioOutput;
    std::unique_ptr<QBuffer> m_mediaBuffer = nullptr;
    std::unique_ptr<ResolvingAudioPlaylist> m_playlist = nullptr;

    QPointer<AudioElement> m_element = nullptr;
    AudioFile::Source m_currentFileSource = AudioFile::Source::Unknown;
};
