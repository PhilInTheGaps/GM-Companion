#include "updatemanager.h"
#include "gm-companion/ui/updatedialog.h"

#include <QXmlStreamReader>
#include <QDebug>

UpdateManager::UpdateManager(int version)
{
    qDebug().noquote() << "Initializing update manager ...";

    // GitHub Release feed
    feedURL = "https://github.com/PhilInTheGaps/GM-Companion/releases.atom";

    currentVersion = version;
    networkManager = new QNetworkAccessManager;
    connect(networkManager, SIGNAL(finished(QNetworkReply *)), this, SLOT(on_networkManager_finished(QNetworkReply *)));

    // Check if openSSL is installed, required for network access to work
    qDebug().noquote() << "Checking SSL installation...";

    if (!QSslSocket::supportsSsl()) qDebug().noquote() << "Please install openSSL";
    else qDebug().noquote() << "SSL is installed.";
}

void UpdateManager::checkForUpdates()
{
    qDebug().noquote() << "Checking for updates ...";
    qDebug().noquote() << "Current version:" << currentVersion;
    qDebug().noquote() << "Releases feed URL:" << feedURL;

    // Get the release feed to check for a new version
    networkManager->get(QNetworkRequest(QUrl(feedURL)));
}

// Evaluate the release feed
void UpdateManager::on_networkManager_finished(QNetworkReply *reply)
{
    qDebug().noquote() << "Finished getting update feed ...";

    QString replyString = reply->readAll();
    QXmlStreamReader reader(replyString);

    int newestVersion = 0;       // 320              Newest Version without dots
    QString newestVersionTitle;  // Beta 3.2         Title of the Version
    QString newestVersionString; // 0.3.2.0          Newest Version with dots

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
                    QString versionString;
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
                    versionString = id.replace("tag:github.com,2008:Repository/78660365/", "");

                    // Remove all the dots to make it a
                    // number and easier to compare
                    int versionNumber = versionString.replace(".", "").toInt();

                    if (versionNumber > newestVersion)
                    {
                        newestVersion      = versionNumber;
                        newestVersionTitle = title;

                        // Forgot why I did not simply use versionString, but
                        // must have had a reason
                        newestVersionString = id.replace("tag:github.com,2008:Repository/78660365/", "");
                    }

                    qDebug().noquote() << " -" << title << "(" << versionString << ")";
                } else reader.skipCurrentElement();
            }
        }
        else reader.raiseError("Incorrect file");
    }

    // Decide if a newer version is available than the one installed
    if (newestVersion > currentVersion)
    {
        // If newer version is available, open a dialog to tell the user
        qDebug().noquote() << "Found a newer version:" << newestVersionTitle;
        UpdateDialog *dialog = new UpdateDialog(newestVersionTitle, newestVersionString);
        dialog->show();
    }
    else
    {
        qDebug().noquote() << "Your version is the newest one.";
    }
}
