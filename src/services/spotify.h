#ifndef SPOTIFY_H
#define SPOTIFY_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QProcess>

#include "spotifyconnectorlocal.h"
#include "spotifyconnectorserver.h"
#include "settings/settingsmanager.h"

class Spotify : public QObject
{
    Q_OBJECT
public:
    static Spotify* getInstance();
    ~Spotify();

    void grant() { m_connector->grantAccess(); }
    bool isGranted() const { return m_connector->isAccessGranted(); }
    Q_INVOKABLE void updateConnector();

    int get(QNetworkRequest request) { return m_connector->get(request); }
    int get(QUrl url) { return m_connector->get(url); }
    int put(QUrl url, QString params = "") { return m_connector->put(url, params); }
    int post(QNetworkRequest request, QByteArray data = "") { return m_connector->post(request, data); }

    int getUniqueRequestId() { return m_connector->getUniqueRequestId(); }
    static int getUriType(QString uri);
    static QString getId(QString uri);

    Q_INVOKABLE void startLibrespot();

public slots:
    void get(QNetworkRequest request, int requestId) { m_connector->get(request, requestId); }

private:
    Spotify();

    static bool instanceFlag;
    static Spotify *single;

    QNetworkAccessManager *m_networkManager = nullptr;
    SpotifyConnector *m_connector = nullptr;
    QProcess m_librespotProcess;
    QMap<QString, int> m_requestMap;

    void handleNetworkError(int id, QNetworkReply::NetworkError error, QByteArray data);
    void setDeviceActive();

    QString getLibrespotPath();

signals:
    void authorized();
    void receivedReply(int id, QNetworkReply::NetworkError error, QByteArray data);
    void wrongPassword();

private slots:
    void onAccessGranted();
    void onReceivedReply(int id, QNetworkReply::NetworkError error, QByteArray data);
    void onReceivedDevices(QByteArray data);
};

#endif // SPOTIFY_H
