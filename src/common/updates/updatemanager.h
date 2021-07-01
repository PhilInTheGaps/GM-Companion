#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include <QNetworkReply>
#include <QString>
#include <QNetworkAccessManager>

#include "version.h"

class UpdateManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString newestVersion READ getNewestVersion NOTIFY updateAvailable)

public:
    UpdateManager();
    Q_INVOKABLE void checkForUpdates();
    Q_INVOKABLE QString getNewestVersion() const { return m_newestVersion; }
    Q_INVOKABLE QString getCurrentVersion() const { return CURRENT_VERSION; }

private slots:
    void onNetworkManagerFinished(QNetworkReply*reply);

private:
    QNetworkAccessManager *networkManager = nullptr;

    QString m_feedURL;
    QString m_newestVersion;

protected:
    static bool compareVersions(const QString &v1, const QString &v2);

signals:
    void updateAvailable();
    void noUpdateAvailable();
};

#endif // UPDATEMANAGER_H
