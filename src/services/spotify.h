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
    int get(QUrl url) { return m_connector->get(QNetworkRequest(url)); }
    int put(QUrl url, QByteArray data = "") { return m_connector->put(QNetworkRequest(url), data); }
    int post(QNetworkRequest request, QByteArray data = "") { return m_connector->post(request, data); }

    int getUniqueRequestId() { return m_connector->getUniqueRequestId(); }

    static int getUriType(const QString& uri);
    static QString getIdFromUri(QString uri);
    static QString getIdFromHref(const QString& href);

    Q_INVOKABLE void startLibrespot();

    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    QString status() const { return m_status; }

public slots:
    void get(QNetworkRequest request, int requestId) { m_connector->get(request, requestId); }

private:
    Spotify();

    static bool instanceFlag;
    static Spotify *single;

    QNetworkAccessManager *m_networkManager = nullptr;
    RESTServiceConnector *m_connector = nullptr;
    QProcess m_librespotProcess;
    QMap<QString, int> m_requestMap;
    bool isLibrespotRunning = false;
    QString m_status = "";

    void handleNetworkError(int id, QNetworkReply::NetworkError error, const QByteArray& data);
    void setDeviceActive();
    void stopLibrespot();

    static QString getLibrespotPath();

signals:
    void authorized();
    void receivedReply(int id, QNetworkReply::NetworkError error, QByteArray data);
    void statusChanged();

private slots:
    void onAccessGranted();
    void onReceivedReply(int id, QNetworkReply::NetworkError error, const QByteArray& data);
    void onReceivedDevices(const QByteArray& data);
    void onLibrespotFinished(const int& exitCode, const QProcess::ExitStatus& exitStatus);
    void onLibrespotError(const QProcess::ProcessError& error);
    void updateStatus(const QString& status);
};

#endif // SPOTIFY_H
