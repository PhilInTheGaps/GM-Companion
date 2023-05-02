#pragma once

#include <QObject>
#include <QPointer>

// I don't exactly know what this include does but if it is not there,
// the application won't compile on some systems.
#include <QQmlContext>

#include "addon.h"
#include "addonrepositorymanager.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"

class AddonManager : public QObject
{
    Q_OBJECT
    READ_PROPERTY(QList<Addon *>, addons)
    AUTO_PROPERTY_VAL2(bool, isLoading, false)

public:
    static auto instance() -> QPointer<AddonManager>;

    Q_INVOKABLE bool setAddonEnabled(Addon *addon, bool value);
    Q_INVOKABLE void refresh();
    Q_INVOKABLE QFuture<void> installAsync(Addon *addon);
    Q_INVOKABLE QFuture<void> updateAsync(Addon *addon);
    Q_INVOKABLE bool uninstall(Addon *addon);

    [[nodiscard]] auto repositoryManager() -> AddonRepositoryManager &;

private slots:
    void onRepoManagerIsLoadingChanged(bool isLoading);

private:
    explicit AddonManager(QObject *parent = nullptr);

    void loadAsync();
    void loadInDir(const QString &dir, bool isLocal);
    auto loadAddonDir(const QString &dir, bool isLocal) -> Addon *;
    auto loadAddonArchive(const QString &path, bool isLocal) -> Addon *;

    static auto installLocalAsync(Addon &addon) -> QFuture<void>;
    auto installRemoteAsync(Addon &addon) -> QFuture<void>;

    void sortAddons();

    static auto getRemotePath(const QString &file = QLatin1String()) -> QString;
    static auto getLocalPath(const QString &file = QLatin1String()) -> QString;

    static auto buildFileName(const Addon &addon, bool useNewVersion) -> QString;
    static auto findAddonJson(const QStringList &files) -> QString;
    static auto ensureInstallationDirExists(const QString &fileName) -> bool;

    [[nodiscard]] auto findAddon(const QString &id) const -> Addon *;

    void applyReleaseInfos();

    inline static QPointer<AddonManager> s_instance = nullptr;
    AddonRepositoryManager m_repoManager;
    QNetworkAccessManager m_networkManager;
};
