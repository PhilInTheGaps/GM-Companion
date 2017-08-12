#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

class UpdateManager : public QObject
{
    Q_OBJECT

public:
    UpdateManager(int version);

    void checkForUpdates();

private slots:
    void on_networkManager_finished(QNetworkReply*reply);

private:
    QNetworkAccessManager *networkManager;

    QString feedURL;

    int currentVersion;
};

#endif // UPDATEMANAGER_H
