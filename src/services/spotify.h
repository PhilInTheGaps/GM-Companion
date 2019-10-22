#ifndef SPOTIFY_H
#define SPOTIFY_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include "o2spotify.h"
#include "o2requestor.h"

#include "src/settings/settingsmanager.h"


class Spotify : public QObject
{
    Q_OBJECT
public:
    static Spotify* getInstance();
    ~Spotify();

    void grant();
    bool isGranted() const { return m_o2spotify->linked(); }
    QString token() const { return m_o2spotify->token(); }

    int get(QNetworkRequest request);
    int get(QUrl url);
    int put(QUrl url, QString params = "");
    int post(QNetworkRequest request, QByteArray data = "");

private:
    Spotify();

    static bool instanceFlag;
    static Spotify *single;

    O2Spotify *m_o2spotify = nullptr;
    QNetworkAccessManager *m_networkManager = nullptr;
    SettingsManager m_sManager;

    QUrl m_authUrl;
    bool m_waitingForAuth = false;

    void forceCurrentMachine();
    void openSpotify();

signals:
    void authorize(QUrl url);
    void authorized();
    void receivedGet(int id, QNetworkReply::NetworkError error, QByteArray data);
    void receivedPut(int id, QNetworkReply::NetworkError error, QByteArray data);
    void receivedPost(int id, QNetworkReply::NetworkError error, QByteArray data);

private slots:
    void onLinkingSucceeded();
    void onOpenBrowser(QUrl url);
    void onForcedCurrentMachine(int id, QNetworkReply::NetworkError error, QByteArray data);

};

#endif // SPOTIFY_H
