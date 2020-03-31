#include "shoptool.h"
#include "logging.h"
#include "filesystem/filemanager.h"
#include "settings/settingsmanager.h"

#include <QSettings>
#include <QQmlContext>
#include <QJsonDocument>

ShopTool::ShopTool(QQmlApplicationEngine *engine, QObject *parent)
    : QObject(parent), qmlEngine(engine)
{
    qCDebug(gmShopsTool()) << "Loading Shop Tool ...";

    shopEditor = new ShopEditor(engine);
    itemModel  = new ItemModel;
    qmlEngine->rootContext()->setContextProperty("shopItemModel", itemModel);

    connect(this,       &ShopTool::currentShopChanged, this, &ShopTool::updateItems);
    connect(shopEditor, &ShopEditor::projectsSaved,    this, &ShopTool::shopEditorSaved);

    findShops();
}

void ShopTool::findShops()
{
    qCDebug(gmShopsShopEditor()) << "Finding shops ...";

    m_projects.clear();

    auto requestId = FileManager::getUniqueRequestId();
    auto context   = new QObject;

    connect(FileManager::getInstance(), &FileManager::receivedFiles, context, [ = ](int id, QList<QByteArray>data) {
        if (id != requestId) return;

        for (auto file : data)
        {
            m_projects.append(new ShopProject(QJsonDocument::fromJson(file).object()));
        }

        if (m_projects.size() > 0) m_currentProject = m_projects[0];
        else m_currentProject = nullptr;

        emit projectsChanged();

        delete context;
    });

    FileManager::getInstance()->getFiles(requestId, SettingsManager::getPath("shops"), "*.shop");
}

/**
 * @brief The shop editor has sent new updated projects
 * @param projects List of projects
 */
void ShopTool::shopEditorSaved(QList<ShopProject *>projects)
{
    // Delete old projects
    for (auto p : m_projects)
    {
        if (p) p->deleteLater();
    }

    // Use new ones
    m_projects = projects;

    if (m_projects.size() > 0) m_currentProject = m_projects[0];

    else m_currentProject = nullptr;

    emit projectsChanged();
    emit categoriesChanged();
    emit shopsChanged();
    emit currentShopChanged();
}

/**
 * @brief Get a list of project names
 * @return Project names
 */
QStringList ShopTool::projects()
{
    QStringList names;

    for (auto p : m_projects)
    {
        if (p) names.append(p->name());
    }

    return names;
}

/**
 * @brief Set the current project
 * @param index Project index
 */
void ShopTool::setCurrentProject(int index)
{
    if ((index > -1) && (index < m_projects.size()))
    {
        m_currentProject = m_projects[index];
    }
    else
    {
        m_currentProject = nullptr;
    }

    emit categoriesChanged();
    emit shopsChanged();
    emit currentShopChanged();
}

/**
 * @brief Get a list of category names
 * @return Category names
 */
QStringList ShopTool::categories()
{
    if (!m_currentProject) return {};

    QStringList categories;

    for (auto c : m_currentProject->categories())
    {
        if (c) categories.append(c->name());
    }

    return categories;
}

/**
 * @brief Set the current category
 * @param index Category index
 */
void ShopTool::setCurrentCategory(int index)
{
    if ((index > -1) && m_currentProject && (m_currentProject->categories().size() > index))
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
QStringList ShopTool::shops()
{
    if (!m_currentProject || !m_currentProject->currentCategory()) return {};

    QStringList shops;

    for (auto s : m_currentProject->currentCategory()->shops())
    {
        if (s) shops.append(s->name());
    }

    return shops;
}

/**
 * @brief Set the current shop
 * @param index Shop index
 */
void ShopTool::setCurrentShop(int index)
{
    if ((index > -1) && m_currentProject && m_currentProject->currentCategory() && (index < m_currentProject->currentCategory()->shops().size()))
    {
        m_currentProject->currentCategory()->setCurrentShop(index);
        emit currentShopChanged();
    }
}

/**
 * @brief Update the item model
 */
void ShopTool::updateItems()
{
    if (m_currentProject && m_currentProject->currentCategory() && m_currentProject->currentCategory()->currentShop())
    {
        itemModel->setElements(m_currentProject->currentCategory()->currentShop()->items());
    }
}
