#ifndef DISCORD_H
#define DISCORD_H

#include "service.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>

class Discord : public Service
{
    Q_OBJECT
    Q_PROPERTY(QString channel READ channel WRITE setChannel NOTIFY channelChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
public:
    static Discord *getInstance();
    ~Discord();

    QString channel() const { return m_channel; }
    void setChannel(const QString& channel) { m_channel = channel; emit channelChanged(); }

    bool enabled() const { return m_enabled; }
    void setEnabled(bool enabled) { m_enabled = enabled; emit enabledChanged(); }

    void post(const QString &endpoint, const QByteArray &data);
    void post(const QString &endpoint, QHttpMultiPart *multipart);

signals:
    void channelChanged();
    void enabledChanged();

public slots:
    void connectService() override;
    void disconnectService() override;
    void testConnection();
    void invite();

private:
    explicit Discord(QObject *parent = nullptr);

    static bool instanceFlag;
    static Discord *single;

    QNetworkAccessManager *m_networkManager = nullptr;

    QString m_channel;
    bool m_enabled = false;

    QString serverUrl() const; 
};

#endif // DISCORD_H
