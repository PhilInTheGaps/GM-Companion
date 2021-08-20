#pragma once

#include <QObject>
#include <QMap>
#include "filecacheentry.h"

class FileCache : public QObject
{
    Q_OBJECT
public:
    explicit FileCache(QObject *parent = nullptr);

    bool tryGetData(const QString &path, QByteArray &data);
    bool createOrUpdateEntry(const QString &path, const QByteArray &data);
    bool removeEntry(const QString &path);
    bool moveEntry(const QString &oldPath, const QString &newPath);
    bool copyEntry(const QString& path, const QString& copy);
    bool checkEntry(const QString& path);

private:
    QMap<QString, FileCacheEntry*> m_entries;
};

