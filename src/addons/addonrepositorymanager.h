#pragma once

#include "addonreleaseinfo.h"
#include "addonrepository.h"
#include "thirdparty/propertyhelper/PropertyHelper.h"
#include <QFuture>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QObject>
#include <QQmlContext>

class AddonRepositoryManager : public QObject
{
    Q_OBJECT

    READ_PROPERTY(QList<AddonRepository *>, repositories)
    AUTO_PROPERTY(bool, isLoading)

public:
    explicit AddonRepositoryManager(QObject *parent = nullptr);

    Q_INVOKABLE bool addRepository(const QString &url);
    Q_INVOKABLE bool removeRepository(int repositoryIndex);

    void fetchAllRepositoryData();

    [[nodiscard]] auto releaseInfos() const -> QVector<AddonReleaseInfo>
    {
        return m_releaseInfos;
    }

private:
    const QStringList defaultRepositories = {}; // TODO

    void loadLocalRepositories();
    void loadDefaultRemoteRepositories();
    void loadAdditionalRepositories();

    [[nodiscard]] auto getRepositoryUrls(bool onlyCustom) const -> QStringList;

    [[nodiscard]] auto fetchRepositoryDataAsync(const QString &url) -> QFuture<QVector<AddonReleaseInfo>>;
    [[nodiscard]] static auto fetchRepositoryDataLocalAsync(const QString &url) -> QFuture<QVector<AddonReleaseInfo>>;
    [[nodiscard]] auto fetchRepositoryDataRemoteAsync(const QString &url) -> QFuture<QVector<AddonReleaseInfo>>;
    [[nodiscard]] static auto parseRepositoryData(const QByteArray &data) -> QVector<AddonReleaseInfo>;

    [[nodiscard]] static auto getNewestCompatibleRelease(const QJsonArray &releases) -> QJsonObject;

    QVector<AddonReleaseInfo> m_releaseInfos;
    QNetworkAccessManager m_networkManager;
};
