#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QStringList>

QStringList getFolders(QString path);

QStringList getFiles(QString folder);

QString cleanText(QString text);

QStringList shuffleStringList(QStringList list);

QString rot13(QString s);

#endif // FUNCTIONS_H
