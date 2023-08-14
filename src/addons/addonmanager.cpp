#include "addonmanager.h"
#include "settings/settingsmanager.h"
#include "utils/fileutils.h"
#include "utils/networkutils.h"
#include <QDir>
#include <QFile>
#include <QFuture>
#include <QJsonDocument>
#include <QLoggingCategory>
#include <QNetworkReply>
#include <QtConcurrent/QtConcurrent>
#include <quazip.h>
#include <quazipfile.h>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmAddonManager, "gm.addons.manager")

AddonManager::AddonManager(QObject *parent) : QObject(parent)
{
    m_networkManager.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
    connect(&m_repoManager, &AddonRepositoryManager::isLoadingChanged, this,
            &AddonManager::onRepoManagerIsLoadingChanged);
    loadAsync();
}

auto AddonManager::instance() -> QPointer<AddonManager>
{
    if (!s_instance)
    {
        s_instance = QPointer(new AddonManager(nullptr));
    }

    return s_instance;
}

auto AddonManager::setAddonEnabled(Addon *addon, bool value) -> bool
{
    if (!addon)
    {
        qCWarning(gmAddonManager()) << "Can not enable/disable addon because it's null!";
        return false;
    }

    addon->enabled(value);
    SettingsManager::instance()->setAddonEnabled(addon->id(), value);

    sortAddons();
    return true;
}

auto AddonManager::installAsync(Addon *addon) -> QFuture<void>
{
    if (!addon) return {};

    qCDebug(gmAddonManager()) << "Installing addon" << addon->id() << "...";

    if (addon->downloadUrl().isEmpty())
    {
        qCWarning(gmAddonManager()) << "Could not install addon" << addon->id() << ", downloadUrl is empty!";
        return {};
    }

    if (NetworkUtils::isHttpUrl(addon->downloadUrl()))
    {
        return installRemoteAsync(*addon);
    }

    return installLocalAsync(*addon);
}

auto AddonManager::installLocalAsync(Addon &addon) -> QFuture<void>
{
    addon.isInstalling(true);

    auto installSync = [&addon]() {
        if (QFile file(addon.downloadUrl()); file.exists())
        {
            auto installFile = getRemotePath(buildFileName(addon, true) + ".zip"_L1);

            if (!ensureInstallationDirExists(installFile))
            {
                addon.isInstalling(false);
                return;
            }

            if (!file.copy(installFile))
            {
                addon.isInstalling(false);
                qCWarning(gmAddonManager()) << "Could not install addon, could not copy file to" << installFile;
                return;
            }

            addon.updateWithReleaseInfo();
            addon.path(installFile);
            qCDebug(gmAddonManager()) << "Installed local addon" << addon.id();
        }
        else
        {
            qCWarning(gmAddonManager()) << "Could not install addon, file" << addon.downloadUrl() << "does not exist";
        }

        addon.isInstalling(false);
    };

    return QtConcurrent::run(installSync);
}

auto AddonManager::installRemoteAsync(Addon &addon) -> QFuture<void>
{
    addon.isInstalling(true);

    auto *reply = m_networkManager.get(QNetworkRequest(QUrl(addon.downloadUrl())));

    auto callback = [&addon, reply]() {
        if (reply->error() != QNetworkReply::NoError)
        {
            qCWarning(gmAddonManager()) << "Could not install addon" << addon.id() << addon.downloadUrl()
                                        << reply->error() << reply->errorString();
            reply->deleteLater();
            return;
        }

        auto installFile = getRemotePath(buildFileName(addon, true) + ".zip"_L1);
        auto data = reply->readAll();
        reply->deleteLater();

        if (!ensureInstallationDirExists(installFile))
        {
            addon.isInstalling(false);
            return;
        }

        QFile file(installFile);
        if (!file.open(QIODevice::WriteOnly))
        {
            addon.isInstalling(false);
            qCWarning(gmAddonManager()) << "Could not install addon, could not write to file" << installFile;
            return;
        }

        file.write(data);
        file.close();

        addon.updateWithReleaseInfo();
        addon.path(installFile);
        addon.isInstalling(false);
        qCDebug(gmAddonManager()) << "Installed remote addon" << addon.id();
    };

    return QtFuture::connect(reply, &QNetworkReply::finished).then(this, callback);
}

/// Update addon by removing the old version and installing the new one
auto AddonManager::updateAsync(Addon *addon) -> QFuture<void>
{
    if (!addon || addon->isLocal() || !addon->isInstalled()) return {};

    if (!uninstall(addon))
    {
        qCWarning(gmAddonManager()) << "Could not remove addon" << addon->id() << "before installing the new version.";
        return {};
    }

    return installAsync(addon);
}

auto AddonManager::uninstall(Addon *addon) -> bool
{
    if (!addon || addon->isLocal() || !addon->isInstalled()) return false;

    setAddonEnabled(addon, false);

    QFile file(addon->path());

    // Modify file permissions, as otherwise we might not have permission to delete it
    // Seems weird, but is necessary under Windows
    file.setPermissions(file.permissions() | QFileDevice::WriteOwner | QFileDevice::WriteUser |
                        QFileDevice::WriteGroup | QFileDevice::WriteOther);

    if (!file.exists())
    {
        qCWarning(gmAddonManager()) << "Could not uninstall addon, file does not exist:" << addon->id()
                                    << addon->path();
        return false;
    }

    if (!file.remove())
    {
        qCWarning(gmAddonManager()) << "Could not uninstall addon, file can not be removed:" << addon->id()
                                    << addon->path() << file.errorString();
        return false;
    }

    addon->path({});

    qCDebug(gmAddonManager()) << "Uninstalled addon" << addon->id();

    applyReleaseInfos();
    return true;
}

auto AddonManager::repositoryManager() -> AddonRepositoryManager &
{
    return m_repoManager;
}

void AddonManager::refresh()
{
    m_repoManager.fetchAllRepositoryData();
}

void AddonManager::onRepoManagerIsLoadingChanged(bool isLoading)
{
    if (isLoading) return;

    applyReleaseInfos();
}

void AddonManager::loadAsync()
{
    qCDebug(gmAddonManager()) << "Loading ...";
    isLoading(true);

    const auto localAddonDirs = {u":/addons"_s, getLocalPath()};

    for (const auto &dir : localAddonDirs)
    {
        loadInDir(dir, true);
    }

    for (const auto &dir : {getRemotePath()})
    {
        loadInDir(dir, false);
    }

    sortAddons();
    refresh();

    isLoading(false);
}

void AddonManager::loadInDir(const QString &dir, bool isLocal)
{
    const auto addonsDir = QDir(dir);
    const auto folders = addonsDir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);

    for (const auto &folder : folders)
    {
        auto *addon = loadAddonDir(FileUtils::fileInDir(folder, addonsDir.absolutePath()), isLocal);

        if (addon)
        {
            addon->isLocal(isLocal);
            a_addons << addon;
        }
    }

    const auto archives = addonsDir.entryList({u"*.zip"_s}, QDir::Files);

    for (const auto &archive : archives)
    {
        auto *addon = loadAddonArchive(FileUtils::fileInDir(archive, addonsDir.absolutePath()), isLocal);

        if (addon)
        {
            addon->isLocal(isLocal);
            a_addons << addon;
        }
    }
}

auto AddonManager::loadAddonDir(const QString &dir, bool isLocal) -> Addon *
{
    QFile file(FileUtils::fileInDir(u"addon.json"_s, dir));

    if (file.exists() && file.open(QIODevice::ReadOnly))
    {
        auto json = QJsonDocument::fromJson(file.readAll());
        file.close();

        auto *addon = Addon::fromJson(this, json, dir, isLocal, Addon::Type::Folder);
        addon->enabled(SettingsManager::instance()->getIsAddonEnabled(addon->id()));
        return addon;
    }

    if (file.exists())
    {
        qCWarning(gmAddonManager()) << "Could not open" << file.fileName();
    }

    return nullptr;
}

auto AddonManager::loadAddonArchive(const QString &path, bool isLocal) -> Addon *
{
    QuaZip zip(path);
    if (!zip.open(QuaZip::mdUnzip))
    {
        qCWarning(gmAddonManager) << "Could not open addon zip file" << path;
        return nullptr;
    }

    const auto addonJson = findAddonJson(zip.getFileNameList());
    if (addonJson.isEmpty())
    {
        qCWarning(gmAddonManager) << "Addon" << path << "does not contain 'addon.json'" << zip.getFileNameList();
        zip.close();
        return nullptr;
    }

    zip.setCurrentFile(addonJson);
    QuaZipFile zipFile(&zip);
    if (!zipFile.open(QIODevice::ReadOnly))
    {
        qCWarning(gmAddonManager) << "Could not open 'addon.json' of addon" << path;
        zip.close();
        return nullptr;
    }

    const auto json = QJsonDocument::fromJson(zipFile.readAll());
    zipFile.close();
    zip.close();

    auto *addon = Addon::fromJson(this, json, path, isLocal, Addon::Type::Archive);
    addon->enabled(SettingsManager::instance()->getIsAddonEnabled(addon->id()));

    return addon;
}

void AddonManager::sortAddons()
{
    std::sort(a_addons.begin(), a_addons.end(), [](const Addon *first, const Addon *second) {
        if (first->enabled() && !second->enabled()) return true;

        if (!first->enabled() && second->enabled()) return false;

        return first->name() > second->name();
    });

    emit addonsChanged(a_addons);
}

auto AddonManager::getRemotePath(const QString &file) -> QString
{
    auto path = u"%1/.gm-companion/addons/remote"_s.arg(QDir::homePath());

    if (file.isEmpty()) return path;

    return FileUtils::fileInDir(file, path);
}

auto AddonManager::getLocalPath(const QString &file) -> QString
{
    auto path = u"%1/.gm-companion/addons/local"_s.arg(QDir::homePath());

    if (file.isEmpty()) return path;

    return FileUtils::fileInDir(file, path);
}

auto AddonManager::buildFileName(const Addon &addon, bool useNewVersion) -> QString
{
    auto name = u"%1_%2"_s.arg(addon.id(), useNewVersion ? addon.newVersion() : addon.version());
    name = name.replace('.', '-');
    return name;
}

auto AddonManager::findAddonJson(const QStringList &files) -> QString
{
    for (const auto &file : files)
    {
        if (file.endsWith("addon.json"_L1)) return file;
    }

    return u""_s;
}

auto AddonManager::ensureInstallationDirExists(const QString &fileName) -> bool
{
    auto installDir = FileUtils::dirFromPath(fileName);

    if (QDir dir(installDir); !dir.exists() && !dir.mkpath(installDir))
    {
        qCWarning(gmAddonManager()) << "Could not install addon, could not create folder" << installDir;
        return false;
    }

    return true;
}

auto AddonManager::findAddon(const QString &id) const -> Addon *
{
    for (auto *addon : qAsConst(a_addons))
    {
        if (addon && addon->id() == id) return addon;
    }

    return nullptr;
}

void AddonManager::applyReleaseInfos()
{
    const auto releaseInfos = m_repoManager.releaseInfos();

    for (const auto &release : releaseInfos)
    {
        auto *addon = findAddon(release.id);

        if (addon)
        {
            addon->setReleaseInfo(release);
        }
        else
        {
            a_addons.append(Addon::fromReleaseInfo(this, release));
        }
    }

    sortAddons();
}
