#include "functions.h"

#include <QDir>

// Gets all folders in directory
QStringList getFolders(QString path){

    QDir myDir(path);
    myDir.setFilter(QDir::Dirs);
    QStringList folders = myDir.entryList();

    return folders;
}

// Gets all files in directory
QStringList getFiles(QString folder){
    QDir myDir(folder);
    myDir.setFilter(QDir::Files);
    QStringList files = myDir.entryList(); //QDir::Files
    return files;
}

// Replaces all undescores with whitespaces and removes .mp3
QString cleanText(QString text){
    text.replace("_", " ");
    text.replace(".mp3", "");
    text.replace(".wav", "");
    text.replace(".png", "");
    text.replace(".jpg", "");
    text.replace(".txt", "");
    text.replace(".ini", "");
    return text;
}
