#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QStringList>

QStringList getFolders(QString path); // Returns all folders in a path
QStringList getFiles(QString folder); // Returns all files in a path

QString cleanText(QString text); // Removes unwanted elements of a string such as underscores or filenames

QStringList shuffleStringList(QStringList list); // Shuffles a QStringList

QString rot13(QString s); // Encrypts a string using the rot13 method, also decrypts encrypted strings

void delay(int seconds);

#endif // FUNCTIONS_H
