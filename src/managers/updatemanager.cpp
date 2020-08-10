#include "updatemanager.h"

#include <QXmlStreamReader>
#include <QDebug>
#include <QSettings>

UpdateManager::UpdateManager()
{
    qDebug().noquote() << "Initializing UpdateManager ...";

    // GitHub Release feed
    m_feedURL = "https://github.com/PhilInTheGaps/GM-Companion/releases.atom";

    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &UpdateManager::onNetworkManagerFinished);

    // Check if openSSL is installed, required for network access to work
    qDebug().noquote() << "Checking SSL installation...";

    if (!QSslSocket::supportsSsl()) qWarning().noquote() << "Please install openSSL";
    else qDebug().noquote() << "SSL is installed.";

    // Get current release version
    QSettings s(":/release.ini", QSettings::IniFormat);
    m_currentVersion = s.value("version", "0.0.0").toString();
}

/**
 * @brief Check the release feed if updates are available
 */
void UpdateManager::checkForUpdates()
{
    qDebug().noquote() << "Checking for updates ...";
    qDebug().noquote() << "Current version:" << m_currentVersion;
    qDebug().noquote() << "Releases feed URL:" << m_feedURL;

    // Get the release feed to check for a new version
    networkManager->get(QNetworkRequest(QUrl(m_feedURL)));
}

/**
 * @brief Get if version 1 is newer than version 2
 * @param v1 Version 1
 * @param v2 Version 2
 * @return True if v1 is newer than v2
 */
bool UpdateManager::compareVersions(QString v1, QString v2)
{
    QStringList split1 = v1.split('.');
    QStringList split2 = v2.split('.');

    for (int i = 0; i < split1.length(); i++)
    {
        auto split11 = split1[i].split('-');
        auto num1 = split11[0].toInt();
        auto dev1 = split11.length() > 1 ? split11[1] : "";

        // 1.2.1 vs 1.2
        if (split2.length() <= i) return true;

        auto split21 = split2[i].split('-');
        auto num2 = split21[0].toInt();
        auto dev2 = split21.length() > 1 ? split21[1] : "";

        // Is version newer?
        if (num1 > num2) return true;

        // Are versions same but have different dev tags?
        if (num1 == num2)
        {
            if (dev1.isEmpty() && !dev2.isEmpty()) return true;

            if (!dev1.isEmpty() && dev2.isEmpty()) return false;

            if (!dev1.isEmpty() && !dev2.isEmpty())
            {
                return QString::compare(dev1, dev2, Qt::CaseInsensitive) > 0;
            }
        }
    }

    return false;
}

/**
 * @brief Evaluate the received release feed
 * @param reply NetworkReply with data from the feed
 */
void UpdateManager::onNetworkManagerFinished(QNetworkReply *reply)
{
    qDebug().noquote() << "Finished getting update feed ...";

    QString replyString = reply->readAll();
    QXmlStreamReader reader(replyString);

    QString newestVersionTitle; // Title of the Version: Beta 3.2
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
                        m_newestVersion    = version;
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
