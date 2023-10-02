#pragma once

#include "multigethelper.h"
#include "results/filecheckresult.h"
#include "results/filedataresult.h"
#include "results/filelistresult.h"
#include "results/filemulticheckresult.h"
#include <QFuture>
#include <QObject>
#include <memory>
#include <utility>

namespace Files
{

class FileAccess
{
public:
    FileAccess() = default;
    Q_DISABLE_COPY_MOVE(FileAccess);

    virtual auto getDataAsync(const QString &path, bool allowCache) -> QFuture<std::shared_ptr<FileDataResult>> = 0;
    virtual auto getDataAsync(const QStringList &paths, bool allowCache)
        -> QFuture<std::vector<std::shared_ptr<FileDataResult>>> = 0;
    virtual auto saveAsync(const QString &path, const QByteArray &data) -> QFuture<std::shared_ptr<FileResult>> = 0;
    virtual auto moveAsync(const QString &oldPath, const QString &newPath) -> QFuture<std::shared_ptr<FileResult>> = 0;
    virtual auto deleteAsync(const QString &path) -> QFuture<std::shared_ptr<FileResult>> = 0;
    virtual auto copyAsync(const QString &path, const QString &copy) -> QFuture<std::shared_ptr<FileResult>> = 0;
    virtual auto listAsync(const QString &path, bool files, bool folders)
        -> QFuture<std::shared_ptr<FileListResult>> = 0;
    virtual auto createDirAsync(const QString &path) -> QFuture<std::shared_ptr<FileResult>> = 0;
    virtual auto checkAsync(const QString &path, bool allowCache) -> QFuture<std::shared_ptr<FileCheckResult>> = 0;
    virtual auto checkAsync(const QStringList &paths, bool allowCache)
        -> QFuture<std::shared_ptr<FileMultiCheckResult>> = 0;

    static auto getInstance() -> std::shared_ptr<FileAccess>
    {
        return instance;
    }

    static void setInstance(std::shared_ptr<FileAccess> fileAccess)
    {
        instance = std::move(fileAccess);
    }

protected:
    auto multiGetDataAsync(std::shared_ptr<MultiGetHelper<FileDataResult>> helper, bool allowCache)
        -> QFuture<std::vector<std::shared_ptr<FileDataResult>>>;
    auto multiCheckAsync(std::shared_ptr<MultiGetHelper<FileCheckResult>> helper, bool allowCache)
        -> QFuture<std::shared_ptr<FileMultiCheckResult>>;
    auto context() -> QObject *;

private:
    inline static std::shared_ptr<FileAccess> instance = nullptr;

    QObject m_context;
};

} // namespace Files
