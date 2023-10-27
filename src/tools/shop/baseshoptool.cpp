#include "baseshoptool.h"
#include "filesystem/file.h"
#include "filesystem/results/filedataresult.h"
#include "filesystem/results/filelistresult.h"
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
        .then(this, [this](Files::FileListResult &&result) { onShopFilesFound(std::move(result)); });
}

void BaseShopTool::onShopFilesFound(Files::FileListResult &&result)
{
    const auto &files = result.filesFull(PROJECT_FILE_GLOB);

    if (!result.success() || files.isEmpty())
    {
        isLoading(false);
        return;
    }

    Files::File::getDataAsync(files).then(
        this, [this](std::vector<Files::FileDataResult> &&results) { onShopFileDataReceived(std::move(results)); });
}

void BaseShopTool::onShopFileDataReceived(std::vector<Files::FileDataResult> &&results)
{
    qCDebug(gmShopsBaseTool()) << "Loading" << results.size() << "projects ...";

    foreach (const auto &result, results)
    {
        a_projects.append(new ShopProject(QJsonDocument::fromJson(result.data()).object(), this));
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
