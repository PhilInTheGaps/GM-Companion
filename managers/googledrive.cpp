#include "googledrive.h"

#include <QProcess>
#include <QDebug>
#include <QDir>

GoogleDrive::GoogleDrive(QObject *parent) : QObject(parent)
{
    qDebug() << "Initializing Google Drive ...";

}

void GoogleDrive::authenticate()
{
    qDebug() << "Authenticating Google Drive ...";

    QProcess::startDetached("python", {"google_drive_auth.py"}, QDir::homePath()+"/.gm-companion/tools/cloud");
}

void GoogleDrive::downloadAll(QString folderID, QString subfolder)
{
    qDebug().noquote() << "Downloading all files in folder with ID:" << folderID << "to subfolder:" << subfolder;

    QProcess::startDetached("python", {"google_drive_download_all.py", folderID, subfolder}, QDir::homePath()+"/.gm-companion/tools/cloud");
}
