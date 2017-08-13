#include "updatemanager.h"
#include "dialogs/updatedialog.h"

#include <QXmlStreamReader>
#include <QDebug>

UpdateManager::UpdateManager(int version)
{
    qDebug().noquote() << "Initializing update manager ...";

    feedURL = "https://github.com/PhilInTheGaps/GM-Companion/releases.atom";

    currentVersion = version;
    networkManager = new QNetworkAccessManager;
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_networkManager_finished(QNetworkReply*)));

    // Check if openSSL is installed
    qDebug().noquote() << "Checking SSL installation...";
    if (!QSslSocket::supportsSsl()){
        qDebug().noquote() << "Please install openSSL";
    }else{
        qDebug().noquote() << "SSL is installed.";
    }
}

void UpdateManager::checkForUpdates()
{
    qDebug().noquote() << "Checking for updates ...";
    qDebug().noquote() << "Current version:" << currentVersion;
    qDebug().noquote() << "Releases feed URL:" << feedURL;

    networkManager->get(QNetworkRequest(QUrl(feedURL)));
}

void UpdateManager::on_networkManager_finished(QNetworkReply *reply)
{
    qDebug().noquote() << "Finished getting update feed ...";

    QString replyString = reply->readAll();
    QXmlStreamReader reader(replyString);

    int newestVersion = 0;
    QString newestVersionTitle;
    QString newestVersionString;

    qDebug() << "Found the following versions:";
    if (reader.readNextStartElement())
    {
        if (reader.name() == "feed")
        {
            while(reader.readNextStartElement())
            {
                if(reader.name() == "entry")
                {
                    QString id;
                    QString versionString;
                    QString title;
                    QString content;

                    while(reader.readNextStartElement()){
                        if (reader.name() == "id"){
                            id = reader.readElementText();
                        }
                        else if (reader.name() == "title"){
                            title = reader.readElementText();
                        }
                        else if (reader.name() == "content"){
                            content = reader.readElementText();
                        }
                        else {
                            reader.skipCurrentElement();
                        }
                    }

                    versionString = id.replace("tag:github.com,2008:Repository/78660365/", "");
                    int versionNumber = versionString.replace(".", "").toInt();

                    if (versionNumber > newestVersion)
                    {
                        newestVersion = versionNumber;
                        newestVersionTitle = title;
                        newestVersionString = id.replace("tag:github.com,2008:Repository/78660365/", "");
                    }

                    qDebug().noquote() << " -" << title << "(" << versionString << ")";

                }else
                    reader.skipCurrentElement();
            }
        }
        else
            reader.raiseError("Incorrect file");
    }

    // Decide if a newer version is available than the one installed
    if (newestVersion > currentVersion)
    {
        qDebug().noquote() << "Found a newer version:" << newestVersionTitle;

        #ifdef _WIN32
        UpdateDialog *dialog = new UpdateDialog(newestVersionTitle, newestVersionString);
        dialog->show();
        #endif
    } else
    {
        qDebug().noquote() << "Your version is the newest one.";
    }
}
