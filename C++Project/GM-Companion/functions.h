#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QStringList>

QStringList getFolders(QString path);

QStringList getFiles(QString folder);

QString cleanText(QString text);

#endif // FUNCTIONS_H