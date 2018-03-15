#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

class UpdateManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString newestVersion READ newestVersion NOTIFY updateAvailable)

public:
    UpdateManager();
    Q_INVOKABLE void checkForUpdates();
    Q_INVOKABLE void setCurrentVersion(int version);
    QString newestVersion();

private slots:
    void on_networkManager_finished(QNetworkReply*reply);

private:
    QNetworkAccessManager *networkManager;

    QString feedURL;
    QString l_newestVersion;
    int currentVersion;

signals:
    void updateAvailable();
    void noUpdateAvailable();
};

#endif // UPDATEMANAGER_H
