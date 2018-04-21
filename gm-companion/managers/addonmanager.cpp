#include "addonmanager.h"

#include <QDebug>
#include <QDir>
#include <QFile>

AddonManager::AddonManager(QObject *parent) : QObject(parent)
{
    qDebug() << "Loading Addon Manager ...";

    connect(&networkManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(fileDownloaded(QNetworkReply *)));

    l_urlString = "https://raw.githubusercontent.com/PhilInTheGaps/GM-Companion/master/gm-companion/Addons/versions.txt";
}

void AddonManager::updateAddonList()
{
    networkManager.get(QNetworkRequest(QUrl(l_urlString)));
}

// Return the addon status:
// 0: Addon is not installed
// 1: Installed, but outdated
// 2: Installed
int AddonManager::addonStatus(QString addon, QString version)
{
    QString folder = QDir::homePath() + "/.gm-companion/addons/" + addon;

    if (QDir(folder).exists())
    {
        QFile f(folder + "/version.txt");

        if (f.open(QIODevice::ReadOnly))
        {
            QString content = f.readAll();
            f.close();

            if (content.replace(".", "").toInt() < version.replace(".", "").toInt())
            {
                return 1;
            }
            else
            {
                return 2;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

void AddonManager::fileDownloaded(QNetworkReply *reply)
{
    QString replyString = reply->readAll();

    l_addonNames.clear();
    l_addonVersions.clear();
    l_addonDescriptions.clear();
    l_addonStatusList.clear();

    if (!replyString.contains("<!DOCTYPE html>"))
    {
        replyString = replyString.replace("\n", "");

        QStringList addons = replyString.split(";");

        for (QString addon : addons)
        {
            QStringList info = addon.split(":");

            l_addonNames.append(info.at(0));
            l_addonVersions.append(info.at(1));
            l_addonDescriptions.append(info.at(2));
            l_addonStatusList.append(addonStatus(info.at(0), info.at(1)));
        }
    }
    else
    {
        qDebug() << "Error: Could not read addon release file, are you connected to the internet?";
    }

    emit addonListChanged();
}

void AddonManager::setIsTestVersion(bool isTest)
{
    if (isTest)
    {
        l_urlString = "https://raw.githubusercontent.com/PhilInTheGaps/GM-Companion/master_release_1_0/gm-companion/Addons/versions.txt";
    }
}

QStringList AddonManager::getAddonNames()
{
    return l_addonNames;
}

QStringList AddonManager::getAddonVersions()
{
    return l_addonVersions;
}

QStringList AddonManager::getAddonDescriptions()
{
    return l_addonDescriptions;
}

QList<int>AddonManager::getAddonStatusList()
{
    return l_addonStatusList;
}

QString AddonManager::getDestinationFolder()
{
    return QDir::homePath() + "/.gm-companion/addons";
}
