#include "filemanager.h"
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <gm-companion/functions.h>
#include <QFileInfo>
#include <QDateTime>

FileManager::FileManager()
{
    // The FileManager is responsible for creating .gm-companion directories and
    // deleting old obsolete files
}

void FileManager::run()
{
    // Local home directory
    QDir lDir(QDir::homePath() + "/.gm-companion");

    // List of all directories to be created
    QStringList dirList = { "addons", "audio", "characters", "maps", "music", "names", "shop", "notes", "radio", "resources", "units", "sounds", "styles", "logs" };

    // List of all files to be deleted
    filesToBeDeleted.append({ "styles/DarkStyle.qss", "styles/DarkStyleTest.qss", "styles/DarkOrange.qss", "styles/Legacy.qss", "styles/Dark.qss", "styles/Whites.qss" });

    // List of all folders to be deleted
    foldersToBeDeleted.append({ "names/DSA5", "names/SIFRP", "names/Generic" });

    // Check if local .gm-companion directory exists and create it if not
    if (!lDir.exists())
    {
        qDebug().noquote() << QDir::homePath() + "/.gm-companion does not exist. Creating...";
        lDir.mkpath(".");
    }

    // Check if subdirs exist and create them if not
    for (QString path : dirList) {
        QDir dir(QDir::homePath() + "/.gm-companion/" + path);

        if (!dir.exists()) {
            qDebug().noquote() << QDir::homePath() + "/.gm-companion/" + path + " does not exist. Creating...";
            dir.mkpath(".");
        }
    }

    // Delete obsolete Files
    if (!filesToBeDeleted.isEmpty())
    {
        for (QString file : filesToBeDeleted)
        {
            QFile f(QDir::homePath() + "/.gm-companion/" + file);

            if (f.exists())
            {
                qDebug().noquote() << "Removing " + QDir::homePath() + "/.gm-companion/" + file << "...";
                f.remove();
            }
        }
    }

    // Delete obsolete Folders
    if (!foldersToBeDeleted.isEmpty())
    {
        for (QString folder : foldersToBeDeleted)
        {
            if (QDir(QDir::homePath() + "/.gm-companion/" + folder).exists())
            {
                qDebug().noquote() << "Removing " + QDir::homePath() + "/.gm-companion/" + folder;
                removeDir(QDir::homePath() + "/.gm-companion/" + folder);
            }
        }
    }

    qDebug() << "Done.";
}

// Removes Directories and Files
bool FileManager::removeDir(const QString& dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists())
    {
        Q_FOREACH (QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
        {
            if (info.isDir()) result = removeDir(info.absoluteFilePath());
            else result = QFile::remove(info.absoluteFilePath());

            if (!result) return result;
        }
        result = QDir().rmdir(dirName);
    }
    return result;
}
