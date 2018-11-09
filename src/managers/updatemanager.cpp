#include "updatemanager.h"

#include <QXmlStreamReader>
#include <QDebug>
#include <QSettings>

UpdateManager::UpdateManager()
{
    qDebug().noquote() << "Initializing update manager ...";

    // GitHub Release feed
    m_feedURL = "https://github.com/PhilInTheGaps/GM-Companion/releases.atom";

    networkManager = new QNetworkAccessManager;
    connect(networkManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(on_networkManager_finished(QNetworkReply *)));

    // Check if openSSL is installed, required for network access to work
    qDebug().noquote() << "Checking SSL installation...";

    if (!QSslSocket::supportsSsl()) qWarning().noquote() << "Please install openSSL";
    else qDebug().noquote() << "SSL is installed.";

    // Get current release version
    QSettings s(":/release.ini", QSettings::IniFormat);
    m_currentVersion = s.value("version", "0.0.0").toString();
}

void UpdateManager::checkForUpdates()
{
    qDebug().noquote() << "Checking for updates ...";
    qDebug().noquote() << "Current version:" << m_currentVersion;
    qDebug().noquote() << "Releases feed URL:" << m_feedURL;

    // Get the release feed to check for a new version
    networkManager->get(QNetworkRequest(QUrl(m_feedURL)));
}

// Returns if v1 is greater than v2
bool UpdateManager::compareVersions(QString v1, QString v2)
{
    QStringList v1_l = v1.split('.');
    QStringList v2_l = v2.split('.');

    for (int i = 0; i < v1_l.size(); i++)
    {
        if (i < v2_l.size())
        {
            if (v1_l[i].toInt() > v2_l[i].toInt())
            {
                return true;
            }
            else if (v1_l[i].toInt() < v2_l[i].toInt())
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    return false;
}

// Evaluate the release feed
void UpdateManager::on_networkManager_finished(QNetworkReply *reply)
{
    qDebug().noquote() << "Finished getting update feed ...";

    QString replyString = reply->readAll();
    QXmlStreamReader reader(replyString);

    QString newestVersionTitle;  // Beta 3.2         Title of the Version
    m_newestVersion = "0.0.0";

    // Release feed is XML, so here is a XML reader
    qDebug() << "Found the following versions:";

    if (reader.readNextStartElement())
    {
        if (reader.name() == "feed")
        {
            while (reader.readNextStartElement())
            {
                if (reader.name() == "entry")
                {
                    QString id;
                    QString version;
                    QString title;
                    QString content;

                    // Read every xml entry and check if it is useful
                    // information
                    while (reader.readNextStartElement())
                    {
                        if (reader.name() == "id") id = reader.readElementText();

                        else if (reader.name() == "title") title = reader.readElementText();

                        else if (reader.name() == "content") content = reader.readElementText();

                        else reader.skipCurrentElement();
                    }

                    // Version is converted from git tag, so we have to remove a
                    // bunch of junk
                    version = id.replace("tag:github.com,2008:Repository/78660365/", "");
                    qDebug() << version;

                    // Compare with current newest version
                    if (compareVersions(version, m_newestVersion))
                    {
                        m_newestVersion      = version;
                        newestVersionTitle = title;
                    }
                } else reader.skipCurrentElement();
            }
        }
        else reader.raiseError("Incorrect file");
    }

    qDebug() << "Newest version:" << m_newestVersion;

    // Decide if a newer version is available than the one installed
    if (compareVersions(m_currentVersion, m_newestVersion))
    {
        qDebug().noquote() << "Your version is the newest one.";
        emit noUpdateAvailable();
    }
    else
    {
        qDebug().noquote() << "Found a newer version:" << newestVersionTitle;
        m_newestVersion = newestVersionTitle;
        emit updateAvailable();
    }
}
