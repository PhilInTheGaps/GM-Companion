#pragma once

#include "filecacheentry.h"
#include <QMap>
#include <QObject>

class FileCache : public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;

    bool tryGetData(const QString &path, QByteArray &data);
    bool createOrUpdateEntry(const QString &path, const QByteArray &data);
    bool removeEntry(const QString &path);
    bool moveEntry(const QString &oldPath, const QString &newPath);
    bool copyEntry(const QString &path, const QString &copy);
    bool checkEntry(const QString &path);

    void printEntries() const;

private:
    QMap<QString, FileCacheEntry *> m_entries;
};
