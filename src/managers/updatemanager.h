#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

class UpdateManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString newestVersion READ getNewestVersion NOTIFY updateAvailable)

public:
    UpdateManager();
    Q_INVOKABLE void checkForUpdates();
    Q_INVOKABLE QString getNewestVersion() const { return m_newestVersion; }
    Q_INVOKABLE QString getCurrentVersion() const { return m_currentVersion; }

private slots:
    void on_networkManager_finished(QNetworkReply*reply);

private:
    QNetworkAccessManager *networkManager;

    QString m_feedURL;
    QString m_newestVersion;
    QString m_currentVersion;

    bool compareVersions(QString v1, QString v2);

signals:
    void updateAvailable();
    void noUpdateAvailable();
};

#endif // UPDATEMANAGER_H
