#pragma once

#include "../project/audioelement.h"
#include "bufferedaudioplayer.h"
#include "spotifyplayer.h"
#include <QPointer>
#include <memory>

namespace Files
{
class FileDataResult;
}

class MusicPlayer : public BufferedAudioPlayer
{
    Q_OBJECT
public:
    explicit MusicPlayer(QNetworkAccessManager *networkManager, MetaDataReader &metaDataReader,
                         QObject *parent = nullptr);

public slots:
    void play(AudioElement *element) override;
    void play() override;
    void pause() override;
    void stop() override;
    void setVolume(int linear, int logarithmic) override;
    void again() override;

private:
    void handleUnsupportedMediaSource(const AudioFile &file) override;
    void loadSpotifyFile(const AudioFile &file);

    SpotifyPlayer m_spotifyPlayer;

private slots:
    void onSpotifySongEnded();
    void onSpotifyStateChanged(AudioPlayer::State state);
    void onStateChanged(AudioPlayer::State state);
};
