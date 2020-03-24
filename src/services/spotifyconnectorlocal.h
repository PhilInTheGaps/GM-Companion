#ifndef SPOTIFYCONNECTORLOCAL_H
#define SPOTIFYCONNECTORLOCAL_H

#include <QObject>
#include <QQueue>
#include <QTimer>
#include "ispotifyconnector.h"
#include "settings/settingsmanager.h"

#include "o2spotify.h"
#include "o2requestor.h"
#include "o0settingsstore.h"

class SpotifyConnectorLocal : public ISpotifyConnector
{
    Q_OBJECT
public:
    SpotifyConnectorLocal(QNetworkAccessManager* networkManager);
    ~SpotifyConnectorLocal();

    void grantAccess();
    bool isAccessGranted() const { return m_o2spotify->linked(); }

    int get(QUrl url) { return get(QNetworkRequest(url)); }
    int get(QNetworkRequest request);
    void get(QNetworkRequest request, int requestId);
    int put(QUrl url, QString params = "");
    int post(QNetworkRequest request, QByteArray data = "");

    int getUniqueRequestId() { return ++m_requestCount; }
    QString getConnectorType() const { return "local"; }

private:
    O2Spotify *m_o2spotify = nullptr;
    O0SettingsStore *m_settingsStore = nullptr;
    SettingsManager m_sManager;

    void handleRateLimit(QPair<QNetworkRequest, int> requestPair);
    void startCooldown(int seconds);
    void dequeueRequests();

    bool m_onCooldown = false;
    int m_requestCount = 0;
    int m_currentRequestCount = 0;
    QMap<int, QPair<QNetworkRequest, int>> m_requestMap;
    QQueue<QPair<QNetworkRequest, int>> m_requestQueue;

private slots:
    void onLinkingSucceeded();
    void onOpenBrowser(QUrl url);

    void onCooldownFinished();
    void onReplyReceived(int id, QNetworkReply::NetworkError error, QByteArray data);
};

#endif // SPOTIFYCONNECTORLOCAL_H
