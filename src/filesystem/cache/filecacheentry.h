#pragma once

#include <QObject>
#include <QTemporaryFile>
#include <QTime>

class FileCache;

class FileCacheEntry : public QObject
{
    Q_OBJECT
public:
    explicit FileCacheEntry(const QByteArray &data, FileCache *parent);

    bool tryGetData(QByteArray& data);
    bool update(const QByteArray& data);

    bool isFresh() const;

private:
    QTime m_lastModified;
    QTemporaryFile m_tempFile;

    static constexpr int EXPIRATION_TIME_MS = 30000;
};

