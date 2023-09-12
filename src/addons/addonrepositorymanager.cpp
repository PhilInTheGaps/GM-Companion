#include "addonrepositorymanager.h"
#include "addonmanager.h"
#include "settings/settingsmanager.h"
#include "updates/updatemanager.h"
#include "utils/networkutils.h"
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

using namespace Qt::Literals::StringLiterals;

constexpr ConstQString REPOSITORY_SETTING = "addonRepositories";
constexpr int MINIMAL_COMPATIBLE_VERSION = 1;

AddonRepositoryManager::AddonRepositoryManager(QObject *parent) : QObject{parent}
{
    m_networkManager.setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    loadLocalRepositories();
    loadDefaultRemoteRepositories();
    loadAdditionalRepositories();
}

/// Method for singleton access in QML
auto AddonRepositoryManager::create(QQmlEngine *qmlEngine, QJSEngine *jsEngine) -> AddonRepositoryManager *
{
    Q_UNUSED(jsEngine)

    if (qmlEngine->thread() != AddonManager::instance()->thread())
    {
        AddonManager::instance()->moveToThread(qmlEngine->thread());
    }

    QJSEngine::setObjectOwnership(&AddonManager::instance()->repositoryManager(), QJSEngine::CppOwnership);
    return &AddonManager::instance()->repositoryManager();
}

auto AddonRepositoryManager::addRepository(const QString &url) -> bool
{
    if (url.isEmpty()) return false;

    for (const auto *repo : qAsConst(a_repositories))
    {
        if (repo && repo->url() == url) return false;
    }

    a_repositories << new AddonRepository(this, url, false);
    emit repositoriesChanged();

    SettingsManager::instance()->set(REPOSITORY_SETTING, getRepositoryUrls(true));
    return true;
}

auto AddonRepositoryManager::removeRepository(int repositoryIndex) -> bool
{
    if (repositoryIndex < 0 || repositoryIndex >= repositories().length() ||
        qAsConst(a_repositories)[repositoryIndex]->isDefault())
        return false;

    auto *repo = a_repositories.takeAt(repositoryIndex);
    emit repositoriesChanged();

    repo->deleteLater();

    SettingsManager::instance()->set(REPOSITORY_SETTING, getRepositoryUrls(true));
    return true;
}

void AddonRepositoryManager::fetchAllRepositoryData()
{
    isLoading(true);

    QList<QFuture<void>> combinator;

    foreach (const auto *repo, repositories())
    {
        auto future =
            fetchRepositoryDataAsync(repo->url()).then(this, [this, repo](const std::vector<AddonReleaseInfo> &info) {
                if (!info.empty())
                {
                    qCDebug(gmAddonRepoManager()) << "Successfully read addon repository" << repo->url();
                }

                m_releaseInfos.insert(m_releaseInfos.end(), info.begin(), info.end());
            });

        combinator << future;
    }

    auto callback = [this](const QList<QFuture<void>> &) { isLoading(false); };
    QtFuture::whenAll(combinator.begin(), combinator.end()).then(this, callback);
}

void AddonRepositoryManager::loadLocalRepositories()
{
    QDir const dir(u":/addon_repositories"_s);

    if (!dir.exists()) return;

    const auto entries = dir.entryList(QDir::Files);
    for (const auto &entry : entries)
    {
        a_repositories << new AddonRepository(this, dir.filePath(entry), true);
    }

    emit repositoriesChanged();
}

void AddonRepositoryManager::loadDefaultRemoteRepositories()
{
    for (const auto &url : qAsConst(defaultRepositories))
    {
        a_repositories << new AddonRepository(this, url, true);
    }

    emit repositoriesChanged();
}

void AddonRepositoryManager::loadAdditionalRepositories()
{
    const auto urls = SettingsManager::instance()->get<QStringList>(REPOSITORY_SETTING, {});

    for (const auto &url : urls)
    {
        a_repositories << new AddonRepository(this, url, false);
    }

    emit repositoriesChanged();
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

auto AddonRepositoryManager::fetchRepositoryDataAsync(const QString &url) -> QFuture<std::vector<AddonReleaseInfo>>
{
    if (NetworkUtils::isHttpUrl(url))
    {
        return fetchRepositoryDataRemoteAsync(url);
    }

    return fetchRepositoryDataLocalAsync(url);
}

auto AddonRepositoryManager::fetchRepositoryDataLocalAsync(const QString &url) -> QFuture<std::vector<AddonReleaseInfo>>
{
    return QtConcurrent::run([url]() {
        QFile file(url);

        if (!file.exists() || !file.open(QIODevice::ReadOnly))
        {
            qCWarning(gmAddonRepoManager) << "Could not read addon repository at" << url;
            return std::vector<AddonReleaseInfo>();
        }

        const auto data = file.readAll();
        file.close();

        return parseRepositoryData(data);
    });
}

auto AddonRepositoryManager::fetchRepositoryDataRemoteAsync(const QString &url)
    -> QFuture<std::vector<AddonReleaseInfo>>
{
    auto *reply = m_networkManager.get(QNetworkRequest(QUrl(url)));

    return QtFuture::connect(reply, &QNetworkReply::finished).then(this, [reply, url]() {
        if (reply->error() != QNetworkReply::NoError)
        {
            qCWarning(gmAddonRepoManager())
                << "Could not read remote addon repository at" << url << reply->error() << reply->errorString();
            reply->deleteLater();
            return std::vector<AddonReleaseInfo>();
        }

        const auto data = reply->readAll();
        reply->deleteLater();

        return parseRepositoryData(data);
    });
}

auto AddonRepositoryManager::parseRepositoryData(const QByteArray &data) -> std::vector<AddonReleaseInfo>
{
    const auto jsonArray = QJsonDocument::fromJson(data).array();
    std::vector<AddonReleaseInfo> result;
    result.reserve(jsonArray.size());

    for (const auto &entry : jsonArray)
    {
        auto release = getNewestCompatibleRelease(entry["releases"_L1].toArray());
        if (release.isEmpty()) continue;

        result.push_back(AddonReleaseInfo(entry["id"_L1].toString(), entry["name"_L1].toString(),
                                          entry["name_short"_L1].toString(), release["version"_L1].toString(),
                                          entry["author"_L1].toString(), entry["description"_L1].toString(),
                                          release["download"_L1].toString()));
    }

    return result;
}

auto AddonRepositoryManager::getNewestCompatibleRelease(const QJsonArray &releases) -> QJsonObject
{
    QJsonObject newest;

    for (const auto &release : releases)
    {
        if (release["compatibility"_L1].toInt() < MINIMAL_COMPATIBLE_VERSION) continue;

        auto version = release["version"_L1].toString();

        if (newest.isEmpty() || UpdateManager::compareVersions(version, newest["version"_L1].toString()))
        {
            newest = release.toObject();
        }
    }

    return newest;
}
