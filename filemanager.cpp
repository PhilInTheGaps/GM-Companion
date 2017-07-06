#include "filemanager.h"
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <functions.h>

FileManager::FileManager(){

}

void FileManager::copyFiles(){
    QDir lDir(QDir::homePath()+"/.gm-companion");
    if (!lDir.exists()){
        qDebug() << QDir::homePath()+"/.gm-companion"+QCoreApplication::translate("FileManager","does not exist. Creating...");
        lDir.mkpath(".");
    }

    QStringList dirList = {"music", "sounds", "maps", "characters", "names", "addons", "resources", "styles", "notes"};

    for (QString path : dirList){
        QDir dir(QDir::homePath()+"/.gm-companion/"+path);
        if (!dir.exists()){
            qDebug() << QDir::homePath()+"/.gm-companion/"+path+QCoreApplication::translate("FileManager", " does not exist. Creating...");
            dir.mkpath(".");
        }
    }

    bool copied = false;
    qDebug() << QCoreApplication::translate("FileManager" ,"Copying files to ") << QDir::homePath()+"/.gm-companion ...";

    // Checks the OS and copies the files to a writable directory accordingly
    #ifdef __linux__
    for (QString folder : getFolders("/usr/share/gm-companion/names")){
        if (!folder.contains(".")){
            QDir dir(QDir::homePath()+"/.gm-companion/names/"+folder);
            if (!dir.exists()){
                dir.mkpath(".");
            }

            for (QString subfolder : getFolders("/usr/share/gm-companion/names/"+folder)){
                if (!subfolder.contains(".")){
                    QDir dir2(QDir::homePath()+"/.gm-companion/names/"+folder+"/"+subfolder);
                    if(!dir2.exists()){
                        dir2.mkpath(".");
                    }

                    for (QString file : getFiles("/usr/share/gm-companion/names/"+folder+"/"+subfolder)){
                        QFile f(QDir::homePath()+"/.gm-companion/names/"+folder+"/"+subfolder+"/"+file);
                        if (!f.exists()){
                            copied = true;
                            QFile::copy("/usr/share/gm-companion/names/"+folder+"/"+subfolder+"/"+file, QDir::homePath()+"/.gm-companion/names/"+folder+"/"+subfolder+"/"+file);
                        }
                    }
                }
            }
        }
    }

    for (QString style : getFiles("/usr/share/gm-companion/styles")){
        QFile f(QDir::homePath()+"/.gm-companion/styles/"+style);
        if (!f.exists()){
            copied = true;
            QFile::copy("/usr/share/gm-companion/styles/"+style, QDir::homePath()+"/.gm-companion/styles/"+style);
        }
    }

    QFile f(QDir::homePath()+"/.gm-companion/units.ini");
    if (!f.exists()){
        copied = true;
        QFile::copy("/usr/share/gm-companion/units.ini", QDir::homePath()+"/.gm-companion/units.ini");
    }

    QFile f2(QDir::homePath()+"/.gm-companion/radios.ini");
    if (!f2.exists()){
        copied = true;
        QFile::copy("/usr/share/gm-companion/radios.ini", QDir::homePath()+"/.gm-companion/radios.ini");
    }
    #elif _WIN32
    for (QString folder : getFolders(QApplication::applicationDirPath()+"/names")){
        if (!folder.contains(".")){
            QDir dir(QDir::homePath()+"/.gm-companion/names/"+folder);
            if (!dir.exists()){
                dir.mkpath(".");
            }

            for (QString subfolder : getFolders(QApplication::applicationDirPath()+"/names/"+folder)){
                if (!subfolder.contains(".")){
                    QDir dir2(QDir::homePath()+"/.gm-companion/names/"+folder+"/"+subfolder);
                    if(!dir2.exists()){
                        dir2.mkpath(".");
                    }

                    for (QString file : getFiles(QApplication::applicationDirPath()+"/names/"+folder+"/"+subfolder)){
                        QFile f(QDir::homePath()+"/.gm-companion/names/"+folder+"/"+subfolder+"/"+file);
                        if (!f.exists()){
                            copied = true;
                            QFile::copy(QApplication::applicationDirPath()+"/names/"+folder+"/"+subfolder+"/"+file, QDir::homePath()+"/.gm-companion/names/"+folder+"/"+subfolder+"/"+file);
                        }
                    }
                }
            }
        }
    }

    for (QString style : getFiles(QApplication::applicationDirPath()+"/styles")){
        QFile f(QDir::homePath()+"/.gm-companion/styles/"+style);
        if (!f.exists()){
            copied = true;
            QFile::copy(QApplication::applicationDirPath()+"/styles/"+style, QDir::homePath()+"/.gm-companion/styles/"+style);
        }
    }

    QFile f(QDir::homePath()+"/.gm-companion/units.ini");
    if (!f.exists()){
        copied = true;
        QFile::copy(QApplication::applicationDirPath()+"/units.ini", QDir::homePath()+"/.gm-companion/units.ini");
    }

    QFile f2(QDir::homePath()+"/.gm-companion/radios.ini");
    if (!f2.exists()){
        copied = true;
        QFile::copy(QApplication::applicationDirPath()+"/radios.ini", QDir::homePath()+"/.gm-companion/radios.ini");
    }


    #else
        qDebug() << QCoreApplication::translate("FileManager","This OS is not supported.");
    #endif

    if (copied){
        qDebug() << QCoreApplication::translate("FileManager","Done.");
    }else{
        qDebug() << QCoreApplication::translate("FileManager","Files already exist.");
    }

}
