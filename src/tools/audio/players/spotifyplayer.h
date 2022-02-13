#ifndef SPOTIFYPLAYER_H
#define SPOTIFYPLAYER_H

#include <QQueue>
#include <QBuffer>
#include <QTimer>
#include <QPointer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <gsl/gsl>
#include <chrono>

#include "rest/restnetworkreply.h"
#include "audioplayer.h"
#include "discordplayer.h"
#include "../project/audioelement.h"
#include "../metadata/metadatareader.h"

class SpotifyPlayer : public AudioPlayer
{
    Q_OBJECT

public:
    SpotifyPlayer(MetaDataReader *mDReader, DiscordPlayer *discordPlayer, QObject *parent = nullptr);

public slots:
    void play(const QString& uri);
    void play() override;
    void pause() override { stop(); }
    void stop() override;
    void pausePlay();
    void next() override;
    void again() override;
    void setVolume(int linear, int logarithmic) override;

private:
    QPointer<MetaDataReader> metaDataReader = nullptr;
    QPointer<DiscordPlayer> m_discordPlayer = nullptr;
    gsl::owner<QTimer*> m_songDurationTimer = nullptr;
    gsl::owner<QTimer*> m_metaDataTimer = nullptr;

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
