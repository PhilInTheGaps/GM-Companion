#include "file.h"
#include "fileaccesslocal.h"
#include "fileaccessnextcloud.h"
#include "fileaccessgoogledrive.h"
#include "logging.h"
#include "settings/settingsmanager.h"

using namespace Files;

auto File::getDataAsync(const QString &path, bool allowCache, FileAccess *fileAccess) -> QFuture<FileDataResult*>
{
    auto access = getFileAccess(fileAccess);
    return access ?
                access->getDataAsync(path, allowCache) :
                QFuture<FileDataResult*>();
}

auto File::getDataAsync(const QStringList &paths, bool allowCache, FileAccess *fileAccess) -> QFuture<QVector<FileDataResult*>>
{
    auto access = getFileAccess(fileAccess);
    return access ?
                access->getDataAsync(paths, allowCache) :
                QFuture<QVector<FileDataResult*>>();
}

auto File::saveAsync(const QString &path, const QByteArray &data, FileAccess *fileAccess) -> QFuture<FileResult*>
{
    auto access = getFileAccess(fileAccess);
    return access ?
                access->saveAsync(path, data) :
                QFuture<FileResult*>();
}

auto File::moveAsync(const QString &oldPath, const QString &newPath, FileAccess *fileAccess) -> QFuture<FileResult*>
{
    auto access = getFileAccess(fileAccess);
    return access ?
                access->moveAsync(oldPath, newPath) :
                QFuture<FileResult*>();
}

auto File::deleteAsync(const QString &path, FileAccess *fileAccess) -> QFuture<FileResult*>
{
    auto access = getFileAccess(fileAccess);
    return access ?
                access->deleteAsync(path) :
                QFuture<FileResult*>();
}

auto File::copyAsync(const QString &path, const QString &copy, FileAccess *fileAccess) -> QFuture<FileResult*>
{
    auto access = getFileAccess(fileAccess);
    return access ?
                access->copyAsync(path, copy) :
                QFuture<FileResult*>();
}

auto File::listAsync(const QString &path, bool files, bool folders, FileAccess *fileAccess) -> QFuture<FileListResult*>
{
    auto access = getFileAccess(fileAccess);
    return access ?
                access->listAsync(path, files, folders) :
                QFuture<FileListResult*>();
}

auto File::createDirAsync(const QString &path, FileAccess *fileAccess) -> QFuture<FileResult*>
{
    auto access = getFileAccess(fileAccess);
    return access ?
                access->createDirAsync(path) :
                QFuture<FileResult*>();
}

auto File::checkAsync(const QString &path, bool allowCache, FileAccess *fileAccess) -> QFuture<FileCheckResult*>
{
    auto access = getFileAccess(fileAccess);
    return access ?
                access->checkAsync(path, allowCache) :
                QFuture<FileCheckResult*>();
}

auto File::checkAsync(const QStringList &paths, bool allowCache, FileAccess *fileAccess) -> QFuture<FileMultiCheckResult*>
{
    auto access = getFileAccess(fileAccess);
    return access ?
                access->checkAsync(paths, allowCache) :
                QFuture<FileMultiCheckResult*>();
}

void File::updateFileAccess()
{
    const auto cloudMode = SettingsManager::getSetting("cloudMode", "local");
    qCDebug(gmFileManager()) << "Setting file access to" << cloudMode;

    if (cloudMode == "NextCloud")
    {
        FileAccess::setInstance(new FileAccessNextcloud(nullptr));
    }
    else if (cloudMode == "GoogleDrive")
    {
        FileAccess::setInstance(new FileAccessGoogleDrive(nullptr));
    }
    else
    {
        FileAccess::setInstance(new FileAccessLocal(nullptr));
    }
}

auto File::getFileAccess(FileAccess *fileAccess) -> FileAccess*
{
    if (fileAccess) return fileAccess;

    if (!FileAccess::getInstance())
    {
        updateFileAccess();
    }

    return FileAccess::getInstance();
}