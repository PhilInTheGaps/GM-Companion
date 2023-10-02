#include "file.h"
#include "fileaccess.h"
#include "fileaccessgoogledrive.h"
#include "fileaccesslocal.h"
#include "fileaccessnextcloud.h"
#include "nextcloud/nextcloud.h"
#include "settings/settingsmanager.h"
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;
using namespace Files;

Q_LOGGING_CATEGORY(gmFileManager, "gm.files.manager")

void File::init(NextCloud *nc, GoogleDrive *gd)
{
    s_nc = nc;
    s_gd = gd;
}

auto File::getDataAsync(const QString &path, bool allowCache, std::shared_ptr<FileAccess> fileAccess)
    -> QFuture<std::shared_ptr<FileDataResult>>
{
    auto access = getFileAccess(fileAccess);
    return access ? access->getDataAsync(path, allowCache) : QFuture<std::shared_ptr<FileDataResult>>();
}

auto File::getDataAsync(const QStringList &paths, bool allowCache, std::shared_ptr<FileAccess> fileAccess)
    -> QFuture<std::vector<std::shared_ptr<FileDataResult>>>
{
    auto access = getFileAccess(fileAccess);
    return access ? access->getDataAsync(paths, allowCache) : QFuture<std::vector<std::shared_ptr<FileDataResult>>>();
}

auto File::saveAsync(const QString &path, const QByteArray &data, std::shared_ptr<FileAccess> fileAccess)
    -> QFuture<std::shared_ptr<FileResult>>
{
    auto access = getFileAccess(fileAccess);
    return access ? access->saveAsync(path, data) : QFuture<std::shared_ptr<FileResult>>();
}

auto File::moveAsync(const QString &oldPath, const QString &newPath, std::shared_ptr<FileAccess> fileAccess)
    -> QFuture<std::shared_ptr<FileResult>>
{
    auto access = getFileAccess(fileAccess);
    return access ? access->moveAsync(oldPath, newPath) : QFuture<std::shared_ptr<FileResult>>();
}

auto File::deleteAsync(const QString &path, std::shared_ptr<FileAccess> fileAccess)
    -> QFuture<std::shared_ptr<FileResult>>
{
    auto access = getFileAccess(fileAccess);
    return access ? access->deleteAsync(path) : QFuture<std::shared_ptr<FileResult>>();
}

auto File::copyAsync(const QString &path, const QString &copy, std::shared_ptr<FileAccess> fileAccess)
    -> QFuture<std::shared_ptr<FileResult>>
{
    auto access = getFileAccess(fileAccess);
    return access ? access->copyAsync(path, copy) : QFuture<std::shared_ptr<FileResult>>();
}

auto File::listAsync(const QString &path, bool files, bool folders, std::shared_ptr<FileAccess> fileAccess)
    -> QFuture<std::shared_ptr<FileListResult>>
{
    auto access = getFileAccess(fileAccess);
    return access ? access->listAsync(path, files, folders) : QFuture<std::shared_ptr<FileListResult>>();
}

auto File::createDirAsync(const QString &path, std::shared_ptr<FileAccess> fileAccess)
    -> QFuture<std::shared_ptr<FileResult>>
{
    auto access = getFileAccess(fileAccess);
    return access ? access->createDirAsync(path) : QFuture<std::shared_ptr<FileResult>>();
}

auto File::checkAsync(const QString &path, bool allowCache, std::shared_ptr<FileAccess> fileAccess)
    -> QFuture<std::shared_ptr<FileCheckResult>>
{
    auto access = getFileAccess(fileAccess);
    return access ? access->checkAsync(path, allowCache) : QFuture<std::shared_ptr<FileCheckResult>>();
}

auto File::checkAsync(const QStringList &paths, bool allowCache, std::shared_ptr<FileAccess> fileAccess)
    -> QFuture<std::shared_ptr<FileMultiCheckResult>>
{
    auto access = getFileAccess(fileAccess);
    return access ? access->checkAsync(paths, allowCache) : QFuture<std::shared_ptr<FileMultiCheckResult>>();
}

void File::updateFileAccess()
{
    const auto cloudMode = SettingsManager::instance()->get(u"cloudMode"_s, u"local"_s);
    qCDebug(gmFileManager()) << "Setting file access to" << cloudMode;

    if (s_nc && cloudMode == "NextCloud"_L1)
    {
        FileAccess::setInstance(std::make_shared<FileAccessNextcloud>(*s_nc));
    }
    else if (s_gd && cloudMode == "GoogleDrive"_L1)
    {
        FileAccess::setInstance(std::make_shared<FileAccessGoogleDrive>(*s_gd));
    }
    else
    {
        FileAccess::setInstance(std::make_shared<FileAccessLocal>());
    }
}

auto File::getFileAccess(std::shared_ptr<FileAccess> fileAccess) -> std::shared_ptr<FileAccess>
{
    if (fileAccess) return fileAccess;

    if (!FileAccess::getInstance())
    {
        updateFileAccess();
    }

    return FileAccess::getInstance();
}
