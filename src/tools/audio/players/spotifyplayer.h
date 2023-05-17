#ifndef SPOTIFYPLAYER_H
#define SPOTIFYPLAYER_H

#include <QBuffer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPointer>
#include <QQueue>
#include <QTimer>
#include <chrono>
#include <gsl/gsl>

#include "../metadata/metadatareader.h"
#include "../project/audioelement.h"
#include "audioplayer.h"
#include "rest/restnetworkreply.h"

class SpotifyPlayer : public AudioPlayer
{
    Q_OBJECT

public:
    SpotifyPlayer(MetaDataReader *mDReader, QObject *parent = nullptr);

public slots:
    void play(const QString &uri);
    void play() override;
    void pause() override
    {
        stop();
    }
    void stop() override;
    void pausePlay();
    void next() override;
    void again() override;
    void setVolume(int linear, int logarithmic) override;

private:
    QPointer<MetaDataReader> metaDataReader = nullptr;
    gsl::owner<QTimer *> m_songDurationTimer = nullptr;
    gsl::owner<QTimer *> m_metaDataTimer = nullptr;

    bool m_isPlaying = false;
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
    void startedPlaying();
    void songEnded();
    void receivedElementIcon(AudioElement *element);
};

#endif // SPOTIFYPLAYER_H
