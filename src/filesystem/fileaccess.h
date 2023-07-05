#pragma once

#include <QFuture>
#include <QObject>

#include "multigethelper.h"

#include "results/filecheckresult.h"
#include "results/filedataresult.h"
#include "results/filelistresult.h"
#include "results/filemulticheckresult.h"

namespace Files
{

class FileAccess : public QObject
{
    Q_OBJECT

public:
    using QObject::QObject;

    virtual auto getDataAsync(const QString &path, bool allowCache) -> QFuture<FileDataResult *> = 0;
    virtual auto getDataAsync(const QStringList &paths, bool allowCache) -> QFuture<std::vector<FileDataResult *>> = 0;
    virtual auto saveAsync(const QString &path, const QByteArray &data) -> QFuture<FileResult *> = 0;
    virtual auto moveAsync(const QString &oldPath, const QString &newPath) -> QFuture<FileResult *> = 0;
    virtual auto deleteAsync(const QString &path) -> QFuture<FileResult *> = 0;
    virtual auto copyAsync(const QString &path, const QString &copy) -> QFuture<FileResult *> = 0;
    virtual auto listAsync(const QString &path, bool files, bool folders) -> QFuture<FileListResult *> = 0;
    virtual auto createDirAsync(const QString &path) -> QFuture<FileResult *> = 0;
    virtual auto checkAsync(const QString &path, bool allowCache) -> QFuture<FileCheckResult *> = 0;
    virtual auto checkAsync(const QStringList &paths, bool allowCache) -> QFuture<FileMultiCheckResult *> = 0;

    static auto getInstance() -> FileAccess *
    {
        return instance;
    }

    static void setInstance(FileAccess *fileAccess)
    {
        if (instance) instance->deleteLater();
        instance = fileAccess;
    }

protected:
    auto multiGetDataAsync(MultiGetHelper<FileDataResult> *helper, bool allowCache)
        -> QFuture<std::vector<FileDataResult *>>;
    auto multiCheckAsync(MultiGetHelper<FileCheckResult> *helper, bool allowCache) -> QFuture<FileMultiCheckResult *>;

private:
    inline static FileAccess *instance = nullptr;
};

} // namespace Files
