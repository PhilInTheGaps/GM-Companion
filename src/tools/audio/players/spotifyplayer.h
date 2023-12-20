#pragma once

#include "../metadata/metadatareader.h"
#include "../project/audioelement.h"
#include "audioplayer.h"
#include <QTimer>
#include <chrono>

class SpotifyPlayer : public AudioPlayer
{
    Q_OBJECT

public:
    SpotifyPlayer(MetaDataReader &mDReader, QObject *parent = nullptr);

    [[nodiscard]] static auto canPlay() -> bool;

public slots:
    void play(const QString &uri);
    void play() override;
    void pause() override;
    void stop() override;
    void pausePlay();
    void next(bool withError) override;
    void again() override;
    void setVolume(int linear, int logarithmic) override;

private:
    MetaDataReader &m_metaDataReader;
    QTimer m_songDurationTimer;
    QTimer m_metaDataTimer;

    int m_volume = 0;
    QString m_currentUri;

    void getCurrentSong();
    void startDurationTimer(std::chrono::milliseconds interval);
    void startMetaDataTimer();

    static auto isSpotifyAvailable() -> bool;

private slots:
    void onDurationTimerTimeout();
    void onMetaDataTimerTimeout();

signals:
    void songNamesChanged();
    void songEnded();
    void receivedElementIcon(AudioElement *element);
};
