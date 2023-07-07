#include "baseshoptool.h"
#include "file.h"
#include "settings/settingsmanager.h"
#include <QJsonDocument>
#include <QLoggingCategory>

using namespace Qt::Literals::StringLiterals;

static constexpr auto PROJECT_FILE_GLOB = "*.shop";

Q_LOGGING_CATEGORY(gmShopsBaseTool, "gm.shops.base")

void BaseShopTool::loadData()
{
    if (isDataLoaded()) return;

    isLoading(true);

    projects({});

    Files::File::listAsync(SettingsManager::getPath(u"shops"_s), true, false)
        .then(this, [this](Files::FileListResult *result) { onShopFilesFound(result); });
}

void BaseShopTool::onShopFilesFound(Files::FileListResult *result)
{
    if (!result)
    {
        isLoading(false);
        return;
    }

    const auto &files = result->filesFull(PROJECT_FILE_GLOB);
    result->deleteLater();

    if (files.isEmpty())
    {
        isLoading(false);
        return;
    }

    Files::File::getDataAsync(files).then(
        this, [this](const std::vector<Files::FileDataResult *> &results) { onShopFileDataReceived(results); });
}

void BaseShopTool::onShopFileDataReceived(const std::vector<Files::FileDataResult *> &results)
{
    qCDebug(gmShopsBaseTool()) << "Loading" << results.size() << "projects ...";

    foreach (auto *result, results)
    {
        a_projects.append(new ShopProject(QJsonDocument::fromJson(result->data()).object(), this));
        result->deleteLater();
    }

    emit projectsChanged(a_projects);
    setFirstProjectAsCurrent();

    setIsDataLoaded(true);
    isLoading(false);
}

void BaseShopTool::setFirstProjectAsCurrent()
{
    currentProject(a_projects.isEmpty() ? nullptr : a_projects.constFirst());
}
