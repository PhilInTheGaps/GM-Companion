#pragma once

#include "../../project/audiofile.h"
#include "services/rest/restreply.h"
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
    static void enqueueRequest(const QString &id, Services::SpotifyUtils::SpotifyType type);

    static void startRequest(Services::SpotifyUtils::SpotifyType type);
    static void receivedRequest(Services::RestReply &&reply, Services::SpotifyUtils::SpotifyType type);
    static void fulfillPromises(const QString &id, const QPixmap &image);
    static auto getBatchIds(Services::SpotifyUtils::SpotifyType type) -> QStringList;

    static auto shouldTimerBeStopped(Services::SpotifyUtils::SpotifyType type) -> bool;
    static void stopTimerEarly(Services::SpotifyUtils::SpotifyType type);
    static void startTimer(Services::SpotifyUtils::SpotifyType type);
    static void initTimers();
    static void initTimer(Services::SpotifyUtils::SpotifyType type);

    static auto getQueue(Services::SpotifyUtils::SpotifyType type) -> QQueue<QString> *;
    static auto getCriticalQueueLength(Services::SpotifyUtils::SpotifyType type) -> int;
    static auto getTimer(Services::SpotifyUtils::SpotifyType type) -> QTimer *;
    static auto getEndpoint(Services::SpotifyUtils::SpotifyType type) -> QString;
    static auto getResultArrayName(Services::SpotifyUtils::SpotifyType type) -> QString;

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

    inline static QHash<QString, QList<QSharedPointer<QPromise<QPixmap>>>> m_pendingFutures;
    inline static bool m_haveTimersBeenInitialized = false;
};
