#include "filemanager.h"
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <functions.h>
#include <QFileInfo>
#include <QDateTime>

FileManager::FileManager(){

}

// Removes Directories and Files
bool removeDir(const QString & dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists()) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = QDir().rmdir(dirName);
    }
    return result;
}

// Copies a file if it is newer than the existing one
void FileManager::copyContents(QString path, QString destination){

    bool copy = true;

    if (QFile(destination).exists()){
        QFileInfo oldInfo (path);
        QDateTime oldTime (oldInfo.lastModified());

        QFileInfo newInfo (destination);
        QDateTime newTime (newInfo.lastModified());

        if (newTime.operator >=(oldTime)){
            copy = false;
        } else {
            QFile(destination).remove();
            qDebug() << path + " is newer, replacing...";
        }

    } else {
        qDebug() << "copying " + path;
    }

    if (copy){
        QFile::copy(path, destination);
    }
}

void FileManager::copyFiles(){
    // Local home directory
    QDir lDir(QDir::homePath()+"/.gm-companion");

    // List of all directories to be created
    QStringList dirList = {"addons", "audio", "characters", "maps", "music", "names", "notes", "radio", "resources", "sounds", "styles", "logs"};

    // List of files to be copied
    QStringList fileList = {"units.ini"};

    // List of all files to be deleted
    filesToBeDeleted.append({"styles/DarkStyle.qss", "styles/DarkStyleTest.qss", "styles/DarkOrange.qss", "styles/Legacy.qss"});

    // List of all folders to be deleted
    foldersToBeDeleted.append({"names/DSA5", "names/SIFRP"});

    // Check if local .gm-companion directory exists and create it if not
    if (!lDir.exists()){
        qDebug() << QDir::homePath()+"/.gm-companion"+QCoreApplication::translate("FileManager","does not exist. Creating...");
        lDir.mkpath(".");
    }

    // Check if subdirs exist and create them if not
    for (QString path : dirList){
        QDir dir(QDir::homePath()+"/.gm-companion/"+path);
        if (!dir.exists()){
            qDebug() << QDir::homePath()+"/.gm-companion/"+path+QCoreApplication::translate("FileManager", " does not exist. Creating...");
            dir.mkpath(".");
        }
    }

    qDebug() << QCoreApplication::translate("FileManager" ,"Copying files to ") << QDir::homePath()+"/.gm-companion ...";

    // Actually copy files to the directories
    #ifdef __linux__
    origPath = "/usr/share/gm-companion";
    #elif _WIN32
    origPath = QApplication::applicationDirPath();
    #else
    origPath = QApplication::applicationDirPath();
    qDebug() << QCoreApplication::translate("FileManager","This OS is not supported.");
    #endif

    // Copy folder contents
    for (QString folder : dirList){
        QString topPathString = origPath + "/" + folder;
        QDir topPath(topPathString);

        if (topPath.exists()){
            for (QString file : getFiles(topPathString)){
                copyContents(topPathString+"/"+file, QDir::homePath()+"/.gm-companion/"+folder+"/"+file);
            }
        }
    }

    // Copy files
    for (QString file : fileList){
        if (QFile(origPath+"/"+file).exists()){
            copyContents(origPath+"/"+file, QDir::homePath()+"/.gm-companion/"+file);
        }
    }

    // Copy names
    for (QString folder : getFolders(+"/names")){
        if (!folder.contains(".")){
            QDir dir(origPath+"/names/"+folder);
            if (!dir.exists()){
                dir.mkpath(".");
            }

            for (QString subfolder : getFolders(origPath+"/names/"+folder)){
                if (!subfolder.contains(".")){
                    QDir dir2(QDir::homePath()+"/.gm-companion/names/"+folder+"/"+subfolder);
                    if(!dir2.exists()){
                        dir2.mkpath(".");
                    }

                    for (QString file : getFiles(QApplication::applicationDirPath()+"/names/"+folder+"/"+subfolder)){
                        QFile f(QDir::homePath()+"/.gm-companion/names/"+folder+"/"+subfolder+"/"+file);
                        if (!f.exists())
                        {
                            QFile::copy(origPath+"/names/"+folder+"/"+subfolder+"/"+file, QDir::homePath()+"/.gm-companion/names/"+folder+"/"+subfolder+"/"+file);
                        }
                    }
                }
            }
        }
    }

    // Delete obsolete Files
    if (!filesToBeDeleted.isEmpty()){
        for (QString file : filesToBeDeleted){
            QFile f (QDir::homePath()+"/.gm-companion/"+file);

            if (f.exists()){
                qDebug().noquote() << "Removing " + QDir::homePath()+"/.gm-companion/"+file << "...";

                f.remove();
            }
        }
    }

    // Delete obsolete Folders
    if (!foldersToBeDeleted.isEmpty()){
        for (QString folder : foldersToBeDeleted){
            if (QDir(QDir::homePath()+"/.gm-companion/"+folder).exists()){
                qDebug().noquote() << "Removing " + QDir::homePath()+"/.gm-companion/"+folder;

                removeDir(QDir::homePath()+"/.gm-companion/"+folder);
            }
        }
    }

    qDebug() << QCoreApplication::translate("FileManager","Done.");

}
