#pragma once

#include "../../project/audiofile.h"
#include "services/rest/restnetworkreply.h"
#include "services/spotify/spotifyutils.h"
#include <QFuture>
#include <QHash>
#include <QPixmap>
#include <QPromise>
#include <QQueue>
#include <QSharedPointer>
#include <QString>
#include <QStringList>
#include <QTimer>

class SpotifyImageLoader
{
public:
    static auto loadImageAsync(AudioFile *audioFile) -> QFuture<QPixmap>;
    static auto loadImageAsync(const QString &uri) -> QFuture<QPixmap>;

private:
    static auto loadPlaylistImageAsync(const QString &id) -> QFuture<QPixmap>;
    static void enqueueRequest(const QString &id, SpotifyUtils::SpotifyType type);

    static void startRequest(SpotifyUtils::SpotifyType type);
    static void receivedRequest(RestNetworkReply *reply, SpotifyUtils::SpotifyType type);
    static auto getBatchIds(SpotifyUtils::SpotifyType type) -> QStringList;

    static auto shouldTimerBeStopped(SpotifyUtils::SpotifyType type) -> bool;
    static void stopTimerEarly(SpotifyUtils::SpotifyType type);
    static void startTimer(SpotifyUtils::SpotifyType type);
    static void initTimers();
    static void initTimer(SpotifyUtils::SpotifyType type);

    static auto getQueue(SpotifyUtils::SpotifyType type) -> QQueue<QString> *;
    static auto getCriticalQueueLength(SpotifyUtils::SpotifyType type) -> int;
    static auto getTimer(SpotifyUtils::SpotifyType type) -> QTimer *;
    static auto getEndpoint(SpotifyUtils::SpotifyType type) -> QString;
    static auto getResultArrayName(SpotifyUtils::SpotifyType type) -> QString;

    inline static auto m_albumTimer = QTimer(nullptr);
    inline static auto m_trackTimer = QTimer(nullptr);
    inline static auto m_artistTimer = QTimer(nullptr);
    inline static auto m_episodeTimer = QTimer(nullptr);
    inline static auto m_showTimer = QTimer(nullptr);

    inline static QQueue<QString> m_albumQueue;
    inline static QQueue<QString> m_trackQueue;
    inline static QQueue<QString> m_artistQueue;
    inline static QQueue<QString> m_episodeQueue;
    inline static QQueue<QString> m_showQueue;

    inline static QHash<QString, QSharedPointer<QPromise<QPixmap>>> m_pendingFutures;
    inline static bool m_haveTimersBeenInitialized = false;

    // batch requests have a maximum amount of ids, depending on the type
    static constexpr int CRITICAL_ALBUM_QUEUE_LENGTH = 20;
    static constexpr int CRITICAL_ARTIST_QUEUE_LENGTH = 50;
    static constexpr int CRITICAL_SHOW_QUEUE_LENGTH = 50;
    static constexpr int CRITICAL_EPISODE_QUEUE_LENGTH = 50;
    static constexpr int CRITICAL_TRACK_QUEUE_LENGTH = 50;

    static constexpr int DEFAULT_TIMEOUT_MS = 200;
    static constexpr int RANDOM_TIMEOUT_MS = 100;
};
