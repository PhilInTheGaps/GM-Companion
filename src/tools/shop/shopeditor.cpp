#include "shopeditor.h"
#include "logging.h"
#include "settings/settingsmanager.h"
#include "filesystem/filemanager.h"

#include <QJsonDocument>
#include <QQmlContext>

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

        emit projectListChanged();
        emit projectChanged();

        delete context;
    });

    FileManager::getInstance()->getFiles(requestId, SettingsManager::getPath("shops"), "*.shop");
}

void ShopEditor::findItems()
{
    qCDebug(gmShopsShopEditor()) << "Finding items ...";

    auto requestId = FileManager::getUniqueRequestId();
    auto context   = new QObject;

    connect(FileManager::getInstance(), &FileManager::receivedFile, context, [ = ](int id, QByteArray data) {
        if (id != requestId) return;

        m_itemGroups = { new ItemGroup(tr("Custom"), QJsonDocument::fromJson(data).object()) };

        if (m_itemGroups.size() > 0) m_currentItemGroup = m_itemGroups[0];
        else m_currentItemGroup = nullptr;

        emit itemGroupsChanged();
        emit itemGroupChanged();

        delete context;
    });

    FileManager::getInstance()->getFile(requestId, SettingsManager::getPath("shops") + "/CustomItems.items");
}

/**
 * @brief Get the names of all shop projects
 * @return List of names
 */
QStringList ShopEditor::projectNames() const
{
    QStringList names;

    for (auto p : m_projects)
    {
        if (p) names.append(p->name());
    }

    return names;
}

/**
 * @brief Get the names of all categories in current project
 * @return List of names
 */
QStringList ShopEditor::categoryNames() const
{
    if (!m_currentProject) return {};

    QStringList names;

    for (auto c : m_currentProject->categories())
    {
        if (c) names.append(c->name());
    }

    return names;
}

/**
 * @brief Get the names of all shops in current category
 * @return List of names
 */
QStringList ShopEditor::shopNames() const
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
    if ((index > -1) && (index < m_projects.size()))
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
    if (m_currentProject && (index > -1) && (index < m_currentProject->categories().size()))
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
    if (m_currentProject && m_currentProject->currentCategory() && (index > -1) && (index < m_currentProject->currentCategory()->shops().size()))
    {
        m_currentProject->currentCategory()->setCurrentShop(index);
    }

    emit shopChanged();
}

/**
 * @brief Get the name of the current shop
 * @return Shop name
 */
QString ShopEditor::name() const
{
    if (!m_currentProject || !m_currentProject->currentCategory() || !m_currentProject->currentCategory()->currentShop()) return "";

    return m_currentProject->currentCategory()->currentShop()->name();
}

/**
 * @brief Set the name of the current shop
 * @param name Shop name
 */
void ShopEditor::setName(QString name)
{
    if (m_currentProject && m_currentProject->currentCategory() && m_currentProject->currentCategory()->currentShop()) m_currentProject->currentCategory()->currentShop()->setName(name);

    madeChanges();
    emit categoryChanged();
    emit shopChanged();
}

/**
 * @brief Get the owner of the current shop
 * @return Shop owner
 */
QString ShopEditor::owner() const
{
    if (!m_currentProject || !m_currentProject->currentCategory() || !m_currentProject->currentCategory()->currentShop()) return "";

    return m_currentProject->currentCategory()->currentShop()->owner();
}

/**
 * @brief Set the owner of the current shop
 * @param owner Shop owner
 */
void ShopEditor::setOwner(QString owner)
{
    if (m_currentProject && m_currentProject->currentCategory() && m_currentProject->currentCategory()->currentShop()) m_currentProject->currentCategory()->currentShop()->setOwner(owner);

    madeChanges();
    emit shopChanged();
}

/**
 * @brief Get the description of the current shop
 * @return Shop description
 */
QString ShopEditor::description() const
{
    if (!m_currentProject || !m_currentProject->currentCategory() || !m_currentProject->currentCategory()->currentShop()) return "";

    return m_currentProject->currentCategory()->currentShop()->description();
}

/**
 * @brief Set the description of the current shop
 * @param description Shop description
 */
void ShopEditor::setDescription(QString description)
{
    if (m_currentProject && m_currentProject->currentCategory() && m_currentProject->currentCategory()->currentShop()) m_currentProject->currentCategory()->currentShop()->setDescription(description);

    madeChanges();
    emit shopChanged();
}

/**
 * @brief Update the item model
 */
void ShopEditor::onShopChanged()
{
    if (!m_currentProject || !m_currentProject->currentCategory() || !m_currentProject->currentCategory()->currentShop())
    {
        itemModel->clear();
        return;
    }

    itemModel->setElements(m_currentProject->currentCategory()->currentShop()->items());
}

/**
 * @brief Move a shop in the list
 * @param positions Positions to move shop by (-1 for up, 1 for down)
 */
void ShopEditor::moveShop(int positions)
{
    qCDebug(gmShopsShopEditor()) << "ShopEditor: Moving shop by" << positions << "position(s) ...";

    if (!m_currentProject || !m_currentProject->currentCategory() || !m_currentProject->currentCategory()->currentShop()) return;

    auto shops = m_currentProject->currentCategory()->shops();
    int  index = shops.indexOf(m_currentProject->currentCategory()->currentShop());

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
    if (!m_currentProject || !m_currentProject->currentCategory() || !m_currentProject->currentCategory()->currentShop()) return;

    auto shops = m_currentProject->currentCategory()->shops();
    int  index = shops.indexOf(m_currentProject->currentCategory()->currentShop());
    auto s     = shops.takeAt(index);

    m_currentProject->currentCategory()->setShops(shops);

    if (index == 0) m_currentProject->currentCategory()->setCurrentShop(0); else m_currentProject->currentCategory()->setCurrentShop(index - 1);

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
    if (!m_currentProject || !m_currentProject->currentCategory() || !m_currentProject->currentCategory()->currentShop()) return;

    auto items = m_currentProject->currentCategory()->currentShop()->items();

    if ((index > -1) && (index < items.size()))
    {
        auto i = items.takeAt(index);
        m_currentProject->currentCategory()->currentShop()->setItems(items);
        emit shopChanged();
        i->deleteLater();
        madeChanges();
    }
}

/**
 * @brief Add an item to the current shop
 * @param index Item index
 */
void ShopEditor::addItem(int index)
{
    if (!m_currentProject || !m_currentProject->currentCategory() || !m_currentProject->currentCategory()->currentShop() || !m_currentItemGroup) return;

    qCDebug(gmShopsShopEditor()) << "ShopEditor: Adding item at index" << index << "...";

    if ((index > -1) && (index < m_items.size()))
    {
        int insert       = -1;
        QString category = m_currentItemGroup->items()[index]->category();
        auto    items    = m_currentProject->currentCategory()->currentShop()->items();

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
        m_currentProject->currentCategory()->currentShop()->setItems(items);
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
    auto basePath = SettingsManager::getPath("shops");

    for (auto project : m_projects)
    {
        FileManager::getInstance()->saveFile(
            basePath + "/" + project->name() + ".shop",
            QJsonDocument(project->toJson()).toJson());
    }

    // Copy all projects and send them to ShopTool
    QList<ShopProject *> copies;

    for (auto project : m_projects)
    {
        copies.append(new ShopProject(project));
    }

    emit projectsSaved(copies);

    // Notify ShopEditor UI
    m_isSaved = true;
    emit isSavedChanged();
    emit showInfoBar(tr("Saved!"));
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
void ShopEditor::createThing(QString name, int index)
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
void ShopEditor::createProject(QString name)
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
void ShopEditor::createCategory(QString name)
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
void ShopEditor::createShop(QString name)
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

/**
 * @brief Get the names of all item groups
 * @return List of names
 */
QStringList ShopEditor::itemGroups() const
{
    QStringList names;

    for (auto g : m_itemGroups)
    {
        if (g) names.append(g->name());
    }

    return names;
}

/**
 * @brief Get the names of all item categories
 * @return List of names
 */
QStringList ShopEditor::itemCategories() const
{
    if (!m_currentItemGroup) return {};

    QStringList names;

    for (auto i : m_currentItemGroup->items())
    {
        if (i && !names.contains(i->category()))
        {
            names.append(i->category());
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
    if ((index > -1) && (index < m_itemGroups.size()))
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

    for (auto i : m_currentItemGroup->items())
    {
        if (i && !m_disabledItemCategories.contains(i->category()))
        {
            m_items.append(i);
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
void ShopEditor::setItemCategoryEnabled(QString category, bool b)
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

    if (m_itemGroups.size() > 0)
    {
        auto old = m_itemGroups.takeAt(0);
        old->deleteLater();
    }

    m_itemGroups.insert(0, group);
    m_currentItemGroup = group;

    emit itemGroupsChanged();
    emit itemGroupChanged();
}
