#include "addonrepositorymanager.h"
#include "settings/settingsmanager.h"
#include "thirdparty/asyncfuture/asyncfuture.h"
#include "updates/updatemanager.h"
#include "utils/stringutils.h"

#include <QDir>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QtConcurrent/QtConcurrent>

Q_LOGGING_CATEGORY(gmAddonRepoManager, "gm.addons.repositories")

using namespace AsyncFuture;

constexpr ConstQString REPOSITORY_SETTING = "addonRepositories";
constexpr int MINIMAL_COMPATIBLE_VERSION = 1;

AddonRepositoryManager::AddonRepositoryManager(QObject *parent) : QObject{parent}, a_isLoading(false)
{
    m_networkManager.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    loadLocalRepositories();
    loadDefaultRemoteRepositories();
    loadAdditionalRepositories();
}

auto AddonRepositoryManager::addRepository(const QString &url) -> bool
{
    if (url.isEmpty()) return false;

    for (const auto *repo : qAsConst(a_repositories))
    {
        if (repo && repo->url() == url) return false;
    }

    a_repositories << new AddonRepository(this, url, false);
    emit repositoriesChanged(a_repositories);

    SettingsManager::instance()->set(REPOSITORY_SETTING, getRepositoryUrls(true));
    return true;
}

auto AddonRepositoryManager::removeRepository(int repositoryIndex) -> bool
{
    if (repositoryIndex < 0 || repositoryIndex >= repositories().length() ||
        qAsConst(a_repositories)[repositoryIndex]->isDefault())
        return false;

    auto *repo = a_repositories.takeAt(repositoryIndex);
    emit repositoriesChanged(a_repositories);

    repo->deleteLater();

    SettingsManager::instance()->set(REPOSITORY_SETTING, getRepositoryUrls(true));
    return true;
}

void AddonRepositoryManager::fetchAllRepositoryData()
{
    isLoading(true);

    auto combinator = combine(AllSettled);
    const auto repos = repositories();

    for (const auto *repo : repos)
    {
        auto future = observe(fetchRepositoryDataAsync(repo->url()))
                          .subscribe([this, repo](const QVector<AddonReleaseInfo> &info) {
                              if (info.length() > 0)
                              {
                                  qCDebug(gmAddonRepoManager()) << "Successfully read addon repository" << repo->url();
                              }

                              m_releaseInfos.append(info);
                          })
                          .future();

        combinator << future;
    }

    combinator.subscribe([this]() { isLoading(false); });
}

void AddonRepositoryManager::loadLocalRepositories()
{
    QDir dir(QStringLiteral(":/addon_repositories"));

    if (!dir.exists()) return;

    const auto entries = dir.entryList(QDir::Files);
    for (const auto &entry : entries)
    {
        a_repositories << new AddonRepository(this, dir.filePath(entry), true);
    }

    emit repositoriesChanged(a_repositories);
}

void AddonRepositoryManager::loadDefaultRemoteRepositories()
{
    for (const auto &url : qAsConst(defaultRepositories))
    {
        a_repositories << new AddonRepository(this, url, true);
    }

    emit repositoriesChanged(a_repositories);
}

void AddonRepositoryManager::loadAdditionalRepositories()
{
    const auto urls = SettingsManager::instance()->get<QStringList>(REPOSITORY_SETTING, {});

    for (const auto &url : urls)
    {
        a_repositories << new AddonRepository(this, url, false);
    }

    emit repositoriesChanged(a_repositories);
}

auto AddonRepositoryManager::getRepositoryUrls(bool onlyCustom) const -> QStringList
{
    const auto repos = repositories();
    QStringList urls;
    urls.reserve(repos.length());

    for (const auto *repo : repos)
    {
        if (!repo) continue;

        if (onlyCustom && repo->isDefault()) continue;

        urls << repo->url();
    }

    return urls;
}

auto AddonRepositoryManager::fetchRepositoryDataAsync(const QString &url) -> QFuture<QVector<AddonReleaseInfo>>
{
    if (url.startsWith(QStringLiteral("http://")) || url.startsWith(QStringLiteral("https://")))
    {
        return fetchRepositoryDataRemoteAsync(url);
    }

    return fetchRepositoryDataLocalAsync(url);
}

auto AddonRepositoryManager::fetchRepositoryDataLocalAsync(const QString &url) -> QFuture<QVector<AddonReleaseInfo>>
{
    return QtConcurrent::run([url]() {
        QFile file(url);

        if (!file.exists() || !file.open(QIODevice::ReadOnly))
        {
            qCWarning(gmAddonRepoManager) << "Could not read addon repository at" << url;
            return QVector<AddonReleaseInfo>();
        }

        const auto data = file.readAll();
        file.close();

        return parseRepositoryData(data);
    });
}

auto AddonRepositoryManager::fetchRepositoryDataRemoteAsync(const QString &url) -> QFuture<QVector<AddonReleaseInfo>>
{
    auto *reply = m_networkManager.get(QNetworkRequest(QUrl(url)));

    return observe(reply, &QNetworkReply::finished)
        .subscribe([reply, url]() {
            if (reply->error() != QNetworkReply::NoError)
            {
                qCWarning(gmAddonRepoManager())
                    << "Could not read remote addon repository at" << url << reply->error() << reply->errorString();
                reply->deleteLater();
                return QVector<AddonReleaseInfo>();
            }

            const auto data = reply->readAll();
            reply->deleteLater();

            return parseRepositoryData(data);
        })
        .future();
}

auto AddonRepositoryManager::parseRepositoryData(const QByteArray &data) -> QVector<AddonReleaseInfo>
{
    const auto jsonArray = QJsonDocument::fromJson(data).array();
    QVector<AddonReleaseInfo> result;

    for (const auto &entry : jsonArray)
    {
        auto release = getNewestCompatibleRelease(entry.toObject()[QStringLiteral("releases")].toArray());
        if (release.isEmpty()) continue;

        result << AddonReleaseInfo(
            entry.toObject()[QStringLiteral("id")].toString(), entry.toObject()[QStringLiteral("name")].toString(),
            entry.toObject()[QStringLiteral("name_short")].toString(), release[QStringLiteral("version")].toString(),
            entry.toObject()[QStringLiteral("author")].toString(),
            entry.toObject()[QStringLiteral("description")].toString(), release[QStringLiteral("download")].toString());
    }

    return result;
}

auto AddonRepositoryManager::getNewestCompatibleRelease(const QJsonArray &releases) -> QJsonObject
{
    QJsonObject newest;

    for (const auto &release : releases)
    {
        if (release.toObject()[QStringLiteral("compatibility")].toInt() < MINIMAL_COMPATIBLE_VERSION) continue;

        auto version = release.toObject()[QStringLiteral("version")].toString();

        if (newest.isEmpty() || UpdateManager::compareVersions(version, newest[QStringLiteral("version")].toString()))
        {
            newest = release.toObject();
        }
    }

    return newest;
}
