#pragma once

#include "addonreleaseinfo.h"
#include "addonrepository.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QFuture>
#include <QJSEngine>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QObject>
#include <QQmlContext>
#include <QQmlEngine>
#include <QtQml/qqmlregistration.h>

class AddonRepositoryManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    READ_LIST_PROPERTY(AddonRepository, repositories)
    AUTO_PROPERTY_VAL2(bool, isLoading, false)

public:
    AddonRepositoryManager() = delete;
    explicit AddonRepositoryManager(QObject *parent);
    static auto create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> AddonRepositoryManager *;

    Q_INVOKABLE bool addRepository(const QString &url);
    Q_INVOKABLE bool removeRepository(int repositoryIndex);

    void fetchAllRepositoryData();

    [[nodiscard]] auto releaseInfos() const -> std::vector<AddonReleaseInfo>
    {
        return m_releaseInfos;
    }

private:
    const QStringList defaultRepositories = {"https://raw.githubusercontent.com/gm-companion/addons/main/addons.json"};

    void loadLocalRepositories();
    void loadDefaultRemoteRepositories();
    void loadAdditionalRepositories();

    [[nodiscard]] auto getRepositoryUrls(bool onlyCustom) const -> QStringList;

    [[nodiscard]] auto fetchRepositoryDataAsync(const QString &url) -> QFuture<std::vector<AddonReleaseInfo>>;
    [[nodiscard]] static auto fetchRepositoryDataLocalAsync(const QString &url)
        -> QFuture<std::vector<AddonReleaseInfo>>;
    [[nodiscard]] auto fetchRepositoryDataRemoteAsync(const QString &url) -> QFuture<std::vector<AddonReleaseInfo>>;
    [[nodiscard]] static auto parseRepositoryData(const QByteArray &data) -> std::vector<AddonReleaseInfo>;

    [[nodiscard]] static auto getNewestCompatibleRelease(const QJsonArray &releases) -> QJsonObject;

    std::vector<AddonReleaseInfo> m_releaseInfos;
    QNetworkAccessManager m_networkManager;
};
