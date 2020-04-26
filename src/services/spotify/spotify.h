#ifndef SPOTIFY_H
#define SPOTIFY_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QProcess>

#include "spotifyconnectorlocal.h"
#include "spotifyconnectorserver.h"
#include "settings/settingsmanager.h"
#include "../service.h"

class Spotify : public Service
{
    Q_OBJECT
public:
    static Spotify* getInstance();
    ~Spotify();

    void grant() { m_connector->grantAccess(); }
    bool isGranted() const { return m_connector->isAccessGranted(); }

    int get(QNetworkRequest request) { return connected() ? m_connector->get(request) : 0; }
    int get(QUrl url) { return connected() ? m_connector->get(QNetworkRequest(url)) : 0; }
    int put(QUrl url, QByteArray data = "") { return connected() ? m_connector->put(QNetworkRequest(url), data) : 0; }
    int post(QNetworkRequest request, QByteArray data = "") { return connected() ? m_connector->post(request, data) : 0; }

    int getUniqueRequestId() { return m_connector->getUniqueRequestId(); }

    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    QString username() const { return m_username; }
    void setUsername(const QString& username) { m_username = username; emit usernameChanged(); }

public slots:
    void get(QNetworkRequest request, int requestId) { if (connected()) m_connector->get(request, requestId); }
    void connectService() override;
    void disconnectService() override;

private:
    explicit Spotify(QObject* parent = nullptr);

    static bool instanceFlag;
    static Spotify *single;

    QNetworkAccessManager *m_networkManager = nullptr;
    RESTServiceConnector *m_connector = nullptr;
    QProcess m_librespotProcess;
    QMap<QString, int> m_requestMap;
    bool isLibrespotRunning = false;
    QString m_username;

    void handleNetworkError(int id, QNetworkReply::NetworkError error, const QByteArray& data);
    void setDeviceActive();
    void stopLibrespot();

    static QString getLibrespotPath();
    static QString getLibrespotBinaryName();

    void updateConnector();
    bool startLibrespot();

signals:
    void authorized();
    void receivedReply(int id, QNetworkReply::NetworkError error, QByteArray data);
    void usernameChanged();

private slots:
    void onAccessGranted();
    void onReceivedReply(int id, QNetworkReply::NetworkError error, const QByteArray& data);
    void onReceivedDevices(const QByteArray& data);
    void onLibrespotFinished(const int& exitCode, const QProcess::ExitStatus& exitStatus);
    void onLibrespotError(const QProcess::ProcessError& error);
};

#endif // SPOTIFY_H
