#include "shoptool.h"
#include "logging.h"
#include "settings/settingsmanager.h"
#include "thirdparty/asyncfuture/asyncfuture.h"
#include "utils/utils.h"
#include "utils/fileutils.h"

#include <QQmlContext>
#include <QJsonDocument>

using namespace AsyncFuture;

ShopTool::ShopTool(QQmlApplicationEngine *engine, QObject *parent)
    : AbstractTool(parent), qmlEngine(engine)
{
    qCDebug(gmShopsTool()) << "Loading Shop Tool ...";

    shopEditor = new ShopEditor(engine);
    itemModel  = new ItemModel;
    engine->rootContext()->setContextProperty("shop_tool", this);
    engine->rootContext()->setContextProperty("shop_editor", shopEditor);
    engine->rootContext()->setContextProperty("shopItemModel", itemModel);

    connect(this,       &ShopTool::currentShopChanged, this, &ShopTool::updateItems);
    connect(shopEditor, &ShopEditor::projectsSaved,    this, &ShopTool::shopEditorSaved);
}

void ShopTool::loadData()
{
    if (isDataLoaded()) return;

    setIsDataLoaded(true);
    findShops();
}

void ShopTool::findShops()
{
    qCDebug(gmShopsShopEditor()) << "Finding shops ...";

    m_projects.clear();

    observe(Files::File::listAsync(SettingsManager::getPath("shops"), true, false))
            .subscribe([this](Files::FileListResult *result) { onShopFilesFound(result); });
}

void ShopTool::onShopFilesFound(Files::FileListResult *result)
{
    if (!result) return;

    const auto &files = result->filesFull(PROJECT_FILE_GLOB);
    result->deleteLater();

    observe(Files::File::getDataAsync(files))
            .subscribe([this](const QVector<Files::FileDataResult*> &results)
    {
        onShopFileDataReceived(results);
    });
}

void ShopTool::onShopFileDataReceived(const QVector<Files::FileDataResult *> &results)
{
    for (auto *result : results)
    {
        m_projects.append(new ShopProject(QJsonDocument::fromJson(result->data()).object()));
        result->deleteLater();
    }

    m_currentProject = m_projects.isEmpty() ? nullptr : m_projects[0];

    emit projectsChanged();
}

/**
 * @brief The shop editor has sent new updated projects
 * @param projects List of projects
 */
void ShopTool::shopEditorSaved(const QList<ShopProject *> &projects)
{
    // Delete old projects
    for (auto *project : m_projects)
    {
        if (project) project->deleteLater();
    }

    // Use new ones
    m_projects = projects;
    m_currentProject = m_projects.isEmpty() ? nullptr : m_projects[0];

    emit projectsChanged();
    emit categoriesChanged();
    emit shopsChanged();
    emit currentShopChanged();
}

/**
 * @brief Get a list of project names
 * @return Project names
 */
auto ShopTool::projects() -> QStringList
{
    QStringList names;

    for (auto *project : m_projects)
    {
        if (project) names.append(project->name());
    }

    return names;
}

/**
 * @brief Set the current project
 * @param index Project index
 */
void ShopTool::setCurrentProject(int index)
{
    if (Utils::isInBounds(m_projects, index))
    {
        m_currentProject = m_projects[index];
    }
    else
    {
        m_currentProject = nullptr;
    }

    emit shopsChanged();
    emit currentShopChanged();
    emit categoriesChanged();
}

/**
 * @brief Get a list of category names
 * @return Category names
 */
auto ShopTool::categories() -> QStringList
{
    if (!m_currentProject) return {};

    QStringList categories;

    for (auto *category : m_currentProject->categories())
    {
        if (category) categories.append(category->name());
    }

    return categories;
}

/**
 * @brief Set the current category
 * @param index Category index
 */
void ShopTool::setCurrentCategory(int index)
{
    if (m_currentProject && Utils::isInBounds(m_currentProject->categories(), index))
    {
        m_currentProject->setCurrentCategory(index);
        emit shopsChanged();
        emit currentShopChanged();
    }
}

/**
 * @brief Get the names of all shops in current category
 * @return Shop names
 */
auto ShopTool::shops() -> QStringList
{
    if (!m_currentProject || !m_currentProject->currentCategory()) return {};

    QStringList shops;

    for (auto *shop : m_currentProject->currentCategory()->shops())
    {
        if (shop) shops.append(shop->name());
    }

    return shops;
}

/**
 * @brief Set the current shop
 * @param index Shop index
 */
void ShopTool::setCurrentShop(int index)
{
    if (m_currentProject && m_currentProject->currentCategory()
            && Utils::isInBounds(m_currentProject->currentCategory()->shops(), index))
    {
        m_currentProject->currentCategory()->setCurrentShop(index);
        emit currentShopChanged();
    }
}

auto ShopTool::shopName() const -> QString
{
    if (isCurrentShopValid())
    {
        return currentShop()->name();
    }

    return "";
}

auto ShopTool::shopOwner() const -> QString
{
    if (isCurrentShopValid())
    {
        return currentShop()->owner();
    }

    return "";
}

auto ShopTool::shopDescription() const -> QString
{
    if (isCurrentShopValid())
    {
        return currentShop()->description();
    }

    return "";
}

auto ShopTool::isCurrentShopValid() const -> bool
{
    return m_currentProject
            && m_currentProject->currentCategory()
            && currentShop();
}

auto ShopTool::currentShop() const -> ItemShop*
{
    return m_currentProject->currentCategory()->currentShop();
}

/**
 * @brief Update the item model
 */
void ShopTool::updateItems()
{
    if (isCurrentShopValid())
    {
        itemModel->setElements(currentShop()->items());
    }
}
