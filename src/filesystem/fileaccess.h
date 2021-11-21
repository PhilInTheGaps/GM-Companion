#pragma once

#include <QObject>
#include <QFuture>

#include "multigethelper.h"

#include "results/filedataresult.h"
#include "results/filelistresult.h"
#include "results/filecheckresult.h"
#include "results/filemulticheckresult.h"

namespace Files {

class FileAccess : public QObject
{
    Q_OBJECT

public:
    explicit FileAccess(QObject *parent = nullptr) : QObject(parent) {}

    virtual QFuture<FileDataResult*> getDataAsync(const QString& path, bool allowCache) = 0;
    virtual QFuture<QVector<FileDataResult*>> getDataAsync(const QStringList& paths, bool allowCache) = 0;
    virtual QFuture<FileResult*> saveAsync(const QString& path, const QByteArray& data) = 0;
    virtual QFuture<FileResult*> moveAsync(const QString& oldPath, const QString& newPath) = 0;
    virtual QFuture<FileResult*> deleteAsync(const QString& path) = 0;
    virtual QFuture<FileResult*> copyAsync(const QString& path, const QString& copy) = 0;
    virtual QFuture<FileListResult*> listAsync(const QString& path, bool files, bool folders) = 0;
    virtual QFuture<FileResult*> createDirAsync(const QString& path) = 0;
    virtual QFuture<FileCheckResult*> checkAsync(const QString& path, bool allowCache) = 0;
    virtual QFuture<FileMultiCheckResult*> checkAsync(const QStringList& paths, bool allowCache) = 0;

    static FileAccess *getInstance() { return instance; }

    static void setInstance(FileAccess* fileAccess)
    {
        if (instance) instance->deleteLater();
        instance = fileAccess;
    }

protected:
    QFuture<QVector<FileDataResult*>> getDataAsync(MultiGetHelper<FileDataResult> *helper, bool allowCache);
    QFuture<FileMultiCheckResult*> checkAsync(MultiGetHelper<FileCheckResult> *helper, bool allowCache);

private:
    inline static FileAccess *instance = nullptr;
};

}
