#include "functions.h"

#include <QDir>
#include <QDebug>

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

// Replaces all undescores with whitespaces and removes file extensions
QString cleanText(QString text){
    text.replace("_", " ");
    text.replace(".mp3", "");
    text.replace(".wav", "");
    text.replace(".png", "");
    text.replace(".jpg", "");
    text.replace(".txt", "");
    text.replace(".ini", "");
    text.replace(".qss", "");
    return text;
}

// Randomly shuffles a StringList
QStringList shuffleStringList(QStringList list){
    for (int i = 0; i < list.size()*5; i++){ // The longer the list, the more swaps
        int  index1 = rand() % list.size();
        int  index2 = rand() % list.size();
        list.swap(index1, index2);
    }
    return list;
}

// Takes a string and encrypts it in ROT13 encryption
QString rot13(QString s){
    qDebug() << "Encrypting...";

    QString lowCaps = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";
    QString upperCaps = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ";

    QString encrypted = "";

    for (int i = 0; i<s.length(); i++){
        if (lowCaps.contains(s.at(i))){

            int index = lowCaps.indexOf(s.at(i));
            encrypted.append(lowCaps.at(index+13));

        } else if (upperCaps.contains(s.at(i))){

            int index = upperCaps.indexOf(s.at(i));
            encrypted.append(upperCaps.at(index+13));

        } else {

            encrypted.append(s.at(i));

        }
    }

    qDebug() << "Done.";

    return encrypted;
}
