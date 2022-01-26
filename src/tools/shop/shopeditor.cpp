#include "shopeditor.h"
#include "logging.h"
#include "settings/settingsmanager.h"
#include "utils/utils.h"
#include "utils/fileutils.h"
#include "thirdparty/asyncfuture/asyncfuture.h"

#include <QJsonDocument>
#include <QQmlContext>

using namespace AsyncFuture;

ShopEditor::ShopEditor(QQmlApplicationEngine *engine, QObject *parent)
    : AbstractTool(parent), qmlEngine(engine)
{
    qCDebug(gmShopsShopEditor()) << "Loading Shop Editor ...";
    itemEditor = new ItemEditor(engine);

    itemModel  = new ItemModel;
    itemModel2 = new ItemModel;
    engine->rootContext()->setContextProperty("item_editor", itemEditor);
    engine->rootContext()->setContextProperty("shopEditorItemModel", itemModel);
    engine->rootContext()->setContextProperty("shopEditorItemModel2", itemModel2);

    connect(this,       &ShopEditor::shopChanged,      this, &ShopEditor::onShopChanged);
    connect(this,       &ShopEditor::itemGroupChanged, this, &ShopEditor::onItemsChanged);
    connect(itemEditor, &ItemEditor::itemsSaved,       this, &ShopEditor::itemEditorSaved);
}

void ShopEditor::findShops()
{
    qCDebug(gmShopsShopEditor()) << "Finding shops ...";

    m_projects.clear();

    observe(Files::File::listAsync(SettingsManager::getPath("shops"), true, false))
            .subscribe([this](Files::FileListResult *result) { onShopFilesFound(result); });
}

void ShopEditor::onShopFilesFound(Files::FileListResult *result)
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

void ShopEditor::onShopFileDataReceived(const QVector<Files::FileDataResult*> &results)
{
    for (auto *result : results)
    {
        m_projects.append(new ShopProject(QJsonDocument::fromJson(result->data()).object()));
        result->deleteLater();
    }

    m_currentProject = m_projects.isEmpty() ? nullptr : m_projects[0];

    emit projectListChanged();
    emit projectChanged();
}

void ShopEditor::findItems()
{
    qCDebug(gmShopsShopEditor()) << "Finding items ...";

    const auto path = FileUtils::fileInDir("CustomItems.items", SettingsManager::getPath("shops"));
    observe(Files::File::getDataAsync(path)).subscribe([this](Files::FileDataResult *result) {
        if (!result) return;

        m_itemGroups = {
            new ItemGroup(tr("Custom"),
            QJsonDocument::fromJson(result->data()).object())
        };

        m_currentItemGroup = m_itemGroups.isEmpty() ? nullptr : m_itemGroups[0];

        emit itemGroupsChanged();
        emit itemGroupChanged();

        result->deleteLater();
    });
}

/**
 * @brief Get the names of all shop projects
 * @return List of names
 */
auto ShopEditor::projectNames() const -> QStringList
{
    QStringList names;

    for (auto *project : m_projects)
    {
        if (project) names.append(project->name());
    }

    return names;
}

/**
 * @brief Get the names of all categories in current project
 * @return List of names
 */
auto ShopEditor::categoryNames() const -> QStringList
{
    if (!m_currentProject) return {};

    QStringList names;

    for (auto *category : m_currentProject->categories())
    {
        if (category) names.append(category->name());
    }

    return names;
}

/**
 * @brief Get the names of all shops in current category
 * @return List of names
 */
auto ShopEditor::shopNames() const -> QStringList
{
    if (!m_currentProject || !m_currentProject->currentCategory()) return {};

    QStringList names;

    for (auto s : m_currentProject->currentCategory()->shops())
    {
        if (s) names.append(s->name());
    }

    return names;
}

/**
 * @brief Set the current project
 * @param index Index of project
 */
void ShopEditor::setCurrentProject(int index)
{
    if (Utils::isInBounds(m_projects, index))
    {
        m_currentProject = m_projects[index];
    }
    else
    {
        m_currentProject = nullptr;
    }

    emit projectChanged();
    emit categoryChanged();
    emit shopChanged();
}

/**
 * @brief Set the current category
 * @param index Index of category
 */
void ShopEditor::setCurrentCategory(int index)
{
    if (m_currentProject && Utils::isInBounds(m_currentProject->categories(), index))
    {
        m_currentProject->setCurrentCategory(index);
    }

    emit categoryChanged();
    emit shopChanged();
}

/**
 * @brief Set the current shop
 * @param index Index of shop
 */
void ShopEditor::setCurrentShop(int index)
{
    if (m_currentProject && m_currentProject->currentCategory() &&
            Utils::isInBounds(m_currentProject->currentCategory()->shops(), index))
    {
        m_currentProject->currentCategory()->setCurrentShop(index);
    }

    emit shopChanged();
}

/**
 * @brief Get the name of the current shop
 * @return Shop name
 */
auto ShopEditor::name() const -> QString
{
    if (!isCurrentShopValid()) return "";

    return currentShop()->name();
}

/**
 * @brief Set the name of the current shop
 * @param name Shop name
 */
void ShopEditor::setName(const QString &name)
{
    if (isCurrentShopValid())
    {
        currentShop()->name(name);
    }

    madeChanges();
    emit categoryChanged();
    emit shopChanged();
}

/**
 * @brief Get the owner of the current shop
 * @return Shop owner
 */
auto ShopEditor::owner() const -> QString
{
    if (!isCurrentShopValid()) return "";

    return currentShop()->owner();
}

/**
 * @brief Set the owner of the current shop
 * @param owner Shop owner
 */
void ShopEditor::setOwner(const QString &owner)
{
    if (isCurrentShopValid())
    {
        currentShop()->owner(owner);
    }

    madeChanges();
    emit shopChanged();
}

/**
 * @brief Get the description of the current shop
 * @return Shop description
 */
auto ShopEditor::description() const -> QString
{
    if (!isCurrentShopValid()) return "";

    return currentShop()->description();
}

/**
 * @brief Set the description of the current shop
 * @param description Shop description
 */
void ShopEditor::setDescription(const QString &description)
{
    if (isCurrentShopValid())
    {
        currentShop()->description(description);
    }

    madeChanges();
    emit shopChanged();
}

/**
 * @brief Update the item model
 */
void ShopEditor::onShopChanged()
{
    if (!isCurrentShopValid())
    {
        itemModel->clear();
        return;
    }

    itemModel->setElements(currentShop()->items());
}

/**
 * @brief Move a shop in the list
 * @param positions Positions to move shop by (-1 for up, 1 for down)
 */
void ShopEditor::moveShop(int positions)
{
    qCDebug(gmShopsShopEditor()) << "ShopEditor: Moving shop by" << positions << "position(s) ...";

    if (!isCurrentShopValid()) return;

    auto shops = m_currentProject->currentCategory()->shops();
    int  index = shops.indexOf(currentShop());

    if ((index + positions < shops.size()) && (index + positions > -1))
    {
        shops.move(index, index + positions);
        m_currentProject->currentCategory()->setShops(shops);
        m_currentProject->currentCategory()->setCurrentShop(index + positions);
        emit categoryChanged();

        madeChanges();
    }
}

/**
 * @brief Delete the current shop
 */
void ShopEditor::deleteShop()
{
    if (!isCurrentShopValid()) return;

    auto shops = m_currentProject->currentCategory()->shops();
    int  index = shops.indexOf(currentShop());
    auto s     = shops.takeAt(index);

    m_currentProject->currentCategory()->setShops(shops);
    m_currentProject->currentCategory()->setCurrentShop(index == 0 ? 0 : index - 1);

    emit categoryChanged();
    emit shopChanged();
    madeChanges();

    s->deleteLater();
}

/**
 * @brief Remove an item from the current shop
 * @param index Item index
 */
void ShopEditor::deleteItem(int index)
{
    if (!isCurrentShopValid()) return;

    auto items = currentShop()->items();

    if (Utils::isInBounds(items, index))
    {
        auto *item = items.takeAt(index);
        currentShop()->items(items);
        emit shopChanged();
        item->deleteLater();
        madeChanges();
    }
}

/**
 * @brief Add an item to the current shop
 * @param index Item index
 */
void ShopEditor::addItem(int index)
{
    if (!isCurrentShopValid() || !m_currentItemGroup) return;

    qCDebug(gmShopsShopEditor()) << "ShopEditor: Adding item at index" << index << "...";

    if (Utils::isInBounds(m_items, index))
    {
        int insert       = -1;
        QString category = m_currentItemGroup->items()[index]->category();
        auto    items    = currentShop()->items();

        // Find last item with same category for insertion
        for (int i = 0; i < items.size(); i++)
        {
            if (items[i] && (items[i]->category() == category))
            {
                insert = i + 1;
            }
        }

        if (insert == -1) insert = items.size();

        // Insert copy of item
        items.insert(insert, new Item(m_items[index]));
        currentShop()->items(items);
        emit shopChanged();
        madeChanges();
    }
}

/**
 * @brief Save shops to disc and send copies to shop tool
 */
void ShopEditor::save()
{
    // Save to disc
    const auto basePath = SettingsManager::getPath("shops");

    for (auto *project : m_projects)
    {
        observe(Files::File::saveAsync(
                    FileUtils::fileInDir(project->name() + ".shop", basePath),
                    QJsonDocument(project->toJson()).toJson())
        ).subscribe([this]() {
            // Notify ShopEditor UI
            m_isSaved = true;
            emit isSavedChanged();
            emit showInfoBar(tr("Saved!"));
        }, [this]() {
            emit showInfoBar(tr("Error: Could not save project!"));
        });
    }

    // Copy all projects and send them to ShopTool
    QList<ShopProject *> copies;

    for (auto project : m_projects)
    {
        copies.append(new ShopProject(project));
    }

    emit projectsSaved(copies);
}

/**
 * @brief Notify UI that changes were made
 */
void ShopEditor::madeChanges()
{
    if (!m_currentProject) return;

    m_isSaved = false;
    emit isSavedChanged();
}

/**
 * @brief Create a project, category or shop
 * @param name Name
 * @param index 0: Project, 1: Category, 2: Shop
 */
void ShopEditor::createThing(const QString &name, int index)
{
    switch (index)
    {
    case 0: // Project
        createProject(name);
        break;

    case 1: // Category
        createCategory(name);
        break;

    case 2: // Shop
        createShop(name);
        break;

    default: return;
    }

    madeChanges();
}

/**
 * @brief Create a project
 * @param name Project name
 */
void ShopEditor::createProject(const QString &name)
{
    if (name.isEmpty()) return;

    auto p = new ShopProject(name, {});

    m_projects.append(p);
    emit projectListChanged();

    if (m_projects.size() == 1) setCurrentProject(0);

    madeChanges();
}

/**
 * @brief Create a category in current project
 * @param name Category name
 */
void ShopEditor::createCategory(const QString &name)
{
    if (!m_currentProject || name.isEmpty()) return;

    auto c    = new ShopCategory(name, {});
    auto list = m_currentProject->categories();
    list.append(c);
    m_currentProject->setCategories(list);
    emit projectChanged();
    madeChanges();
}

/**
 * @brief Create a shop in current category
 * @param name Shop name
 */
void ShopEditor::createShop(const QString &name)
{
    if (!m_currentProject || !m_currentProject->currentCategory() || name.isEmpty()) return;

    auto s    = new ItemShop(name, "", "", {});
    auto list = m_currentProject->currentCategory()->shops();
    list.append(s);
    m_currentProject->currentCategory()->setShops(list);
    emit categoryChanged();
    emit shopChanged();
    madeChanges();
}

auto ShopEditor::isCurrentShopValid() const -> bool
{
    return m_currentProject
            && m_currentProject->currentCategory()
            && currentShop();
}

auto ShopEditor::currentShop() const -> ItemShop*
{
    return m_currentProject->currentCategory()->currentShop();
}

/**
 * @brief Get the names of all item groups
 * @return List of names
 */
auto ShopEditor::itemGroups() const -> QStringList
{
    QStringList names;

    for (auto *group : m_itemGroups)
    {
        if (group) names.append(group->name());
    }

    return names;
}

/**
 * @brief Get the names of all item categories
 * @return List of names
 */
auto ShopEditor::itemCategories() const -> QStringList
{
    if (!m_currentItemGroup) return {};

    QStringList names;

    for (auto *item : m_currentItemGroup->items())
    {
        if (item && !names.contains(item->category()))
        {
            names.append(item->category());
        }
    }

    return names;
}

/**
 * @brief Set the current item group
 * @param index Index of item group
 */
void ShopEditor::setCurrentItemGroup(int index)
{
    if (Utils::isInBounds(m_itemGroups, index))
    {
        m_currentItemGroup = m_itemGroups[index];
    }
    else
    {
        m_currentItemGroup = nullptr;
    }

    enableAllItemCategories();
}

/**
 * @brief Update item list model
 */
void ShopEditor::onItemsChanged()
{
    m_items.clear();

    if (!m_currentItemGroup)
    {
        itemModel2->clear();
        return;
    }

    for (auto *item : m_currentItemGroup->items())
    {
        if (item && !m_disabledItemCategories.contains(item->category()))
        {
            m_items.append(item);
        }
    }

    itemModel2->setElements(m_items);
}

/**
 * @brief Set all item categories enabled or not
 * @param b true: enable all, false: disable all
 */
void ShopEditor::enableAllItemCategories(bool b)
{
    if (b)
    {
        m_disabledItemCategories.clear();
    }
    else
    {
        for (auto i : m_currentItemGroup->items())
        {
            if (i && !m_disabledItemCategories.contains(i->category()))
            {
                m_disabledItemCategories.append(i->category());
            }
        }
    }

    emit itemGroupChanged();
}

/**
 * @brief Set a specific item category enabled or not
 * @param category Name of the category
 * @param b true: set enabled, false: set disabled
 */
void ShopEditor::setItemCategoryEnabled(const QString &category, bool b)
{
    if (!b && !m_disabledItemCategories.contains(category))
    {
        m_disabledItemCategories.append(category);
    }
    else
    {
        m_disabledItemCategories.removeOne(category);
    }

    emit itemGroupChanged();
}

void ShopEditor::loadData()
{
    if (m_isDataLoaded) return;

    m_isDataLoaded = true;
    findShops();
    findItems();
    itemEditor->findItems();
}

/**
 * @brief The item has sent a new custom item group
 * @param group Custom item group
 */
void ShopEditor::itemEditorSaved(ItemGroup *group)
{
    qDebug() << "ShopEditor: Received new items from ItemEditor ...";

    if (!m_itemGroups.isEmpty())
    {
        auto old = m_itemGroups.takeAt(0);
        old->deleteLater();
    }

    m_itemGroups.insert(0, group);
    m_currentItemGroup = group;

    emit itemGroupsChanged();
    emit itemGroupChanged();
}
