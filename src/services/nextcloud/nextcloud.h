#ifndef NEXTCLOUD_H
#define NEXTCLOUD_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "../service.h"

class NextCloud : public Service
{
    Q_OBJECT
public:
    static NextCloud *getInstance();
    ~NextCloud();

    Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectedChanged)
    bool connected() const { return m_connected; }
    void setConnected(const bool& connected);

    Q_PROPERTY(QString loginName READ loginName WRITE setLoginName NOTIFY loginNameChanged)
    QString loginName() const { return m_loginName; }
    void setLoginName(const QString& loginName) { m_loginName = loginName; emit loginNameChanged(); }

    Q_PROPERTY(QString serverUrl READ serverUrl WRITE setServerUrl NOTIFY serverUrlChanged)
    QString serverUrl() const { return m_serverUrl; }
    void setServerUrl(const QString& url) { m_serverUrl = url; emit serverUrlChanged(); }

    QNetworkReply *sendDavRequest(const QByteArray& method, const QString &path, const QByteArray& data);

public slots:
    void login();
    void logout();

signals:
    void connectedChanged();
    void loginNameChanged();
    void serverUrlChanged();

private:
    explicit NextCloud(QObject *parent = nullptr);

    static bool instanceFlag;
    static NextCloud *single;

    QNetworkAccessManager *m_networkManager = nullptr;
    int m_authPolls = 0;
    bool m_loggingIn = false;
    bool m_connected = false;

    QString m_loginName = "";
    QString m_appPassword = "";
    QString m_serverUrl = "";

    void startLoginFlow();
    void pollAuthPoint(const QUrl& url, const QString& token);

};

#endif // NEXTCLOUD_H
