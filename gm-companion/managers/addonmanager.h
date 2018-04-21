#ifndef ADDONMANAGER_H
#define ADDONMANAGER_H

#include "gm-companion/settings/settingsmanager.h"

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class AddonManager : public QObject
{
    Q_OBJECT
public:
    explicit AddonManager(QObject *parent = nullptr);

    Q_INVOKABLE void updateAddonList();
    Q_INVOKABLE QStringList getAddonNames();
    Q_INVOKABLE QStringList getAddonVersions();
    Q_INVOKABLE QStringList getAddonDescriptions();
    Q_INVOKABLE QList<int> getAddonStatusList();
    Q_INVOKABLE QString getDestinationFolder();
    Q_INVOKABLE void setIsTestVersion(bool isTest);

signals:
    void addonListChanged();

private:
    SettingsManager sManager;

    QString l_urlString;
    QNetworkAccessManager networkManager;
    int addonStatus(QString addon, QString version);

    QStringList l_addonNames;
    QStringList l_addonVersions;
    QStringList l_addonDescriptions;
    QList<int> l_addonStatusList;

private slots:
    void fileDownloaded(QNetworkReply *reply);
};

#endif // ADDONMANAGER_H
