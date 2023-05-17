#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QPointer>
#include <gsl/gsl>

#ifdef Q_OS_WIN
#include <QTemporaryDir>
#endif

#include "../project/audioelement.h"
#include "audioplayer.h"
#include "filesystem/file.h"
#include "spotifyplayer.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

class MusicPlayer : public AudioPlayer
{
    Q_OBJECT
public:
    explicit MusicPlayer(MetaDataReader *metaDataReader, QObject *parent = nullptr);

    void play(AudioElement *element);
    void setIndex(int index);

    AUTO_PROPERTY(int, playlistIndex);
    READ_PROPERTY(QList<AudioFile *>, playlist)

public slots:
    void play() override;
    void pause() override;
    void stop() override;
    void setVolume(int linear, int logarithmic) override;
    void next() override;
    void again() override;

private:
    SpotifyPlayer *m_spotifyPlayer = nullptr;
    QMediaPlayer *m_mediaPlayer = nullptr;
    AudioElement *m_currentElement = nullptr;
    MetaDataReader *m_metaDataReader = nullptr;

    /// Context object to easily stop file data requests by deleting the object
    QObject *m_fileRequestContext = nullptr;
    QObject *m_playlistLoadingContext = nullptr;

    // QList<AudioFile*> m_playlist;
    AudioFile::Source m_currentFileSource = AudioFile::Source::Unknown;

    QBuffer m_mediaBuffer;
    QString m_fileName;

#ifdef Q_OS_WIN
    QTemporaryDir m_tempDir;
#endif

    void loadMedia(AudioFile *file);
    void loadLocalFile(AudioFile *file);
    void loadWebFile(AudioFile *file);
    void loadSpotifyFile(AudioFile *file);
    void loadYoutubeFile(AudioFile *file);

    auto loadPlaylist() -> QFuture<void>;
    void clearPlaylist();
    void loadTrackNamesAsync();
    static void loadSpotifyTrackNamesAsync(const QList<AudioFile *> &tracks);

    auto loadPlaylistRecursive(int index = 0) -> QFuture<void>;
    auto loadPlaylistRecursiveSpotify(int index, AudioFile *audioFile) -> QFuture<void>;
    void applyShuffleMode();
    void startPlaying();

    void connectPlaybackStateSignals();
    void connectMetaDataSignals(MetaDataReader *metaDataReader);
    void initSpotifyPlayer();

    void printPlaylist() const;

private slots:
    void onMediaPlayerStateChanged();
    void onMediaPlayerMediaStatusChanged();
    void onMediaPlayerError(QMediaPlayer::Error error);
    void onMediaPlayerMediaChanged();
    void onFileReceived(Files::FileDataResult *result);
    void onSpotifySongEnded();

signals:
    void startedPlaying();
    void clearMetaData();
    void metaDataChanged(QMediaPlayer *mediaPlayer);
    void metaDataChanged(const QString &key, const QVariant &value);
    void metaDataChanged(const QByteArray &data);
    void currentIndexChanged();
};

#endif // MUSICPLAYER_H
