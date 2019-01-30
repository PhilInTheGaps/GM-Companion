#include "shopeditor.h"

#include <QDebug>
#include <QQmlContext>

ShopEditor::ShopEditor(FileManager *fManager, QQmlApplicationEngine *engine, QObject *parent)
    : QObject(parent), fileManager(fManager), qmlEngine(engine)
{
    qDebug() << "Loading Shop Editor ...";
    itemEditor = new ItemEditor(fManager, engine);

    itemModel  = new ItemModel;
    itemModel2 = new ItemModel;
    qmlEngine->rootContext()->setContextProperty("shopEditorItemModel", itemModel);
    qmlEngine->rootContext()->setContextProperty("shopEditorItemModel2", itemModel2);

    connect(fManager->getShopFileManager(), &ShopFileManager::receivedEditorShops, this, &ShopEditor::projectsReceived);
    connect(fManager->getShopFileManager(), &ShopFileManager::receivedItems,       this, &ShopEditor::itemsReceived);
    connect(this,                           &ShopEditor::shopChanged,              this, &ShopEditor::onShopChanged);
    connect(this,                           &ShopEditor::itemGroupChanged,         this, &ShopEditor::onItemsChanged);
    connect(itemEditor,                     &ItemEditor::itemsSaved,               this, &ShopEditor::itemEditorSaved);

    fManager->getShopFileManager()->findShops(fManager->getModeInt(), true);
    fManager->getShopFileManager()->findItems(fManager->getModeInt(), false);
}

void ShopEditor::projectsReceived(QList<ShopProject *>projects)
{
    m_projects = projects;

    if (projects.size() > 0) m_currentProject = projects[0];
    else m_currentProject = nullptr;

    emit projectListChanged();
    emit projectChanged();
}

void ShopEditor::itemsReceived(QList<ItemGroup *>groups)
{
    m_itemGroups = groups;

    if (groups.size() > 0) m_currentItemGroup = groups[0];
    else m_currentItemGroup = nullptr;

    emit itemGroupsChanged();
    emit itemGroupChanged();
}

QStringList ShopEditor::projectNames() const
{
    QStringList names;

    for (auto p : m_projects)
    {
        if (p) names.append(p->name());
    }

    return names;
}

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

void ShopEditor::setCurrentCategory(int index)
{
    if (m_currentProject && (index > -1) && (index < m_currentProject->categories().size()))
    {
        m_currentProject->setCurrentCategory(index);
    }

    emit categoryChanged();
    emit shopChanged();
}

void ShopEditor::setCurrentShop(int index)
{
    if (m_currentProject && m_currentProject->currentCategory() && (index > -1) && (index < m_currentProject->currentCategory()->shops().size()))
    {
        m_currentProject->currentCategory()->setCurrentShop(index);
    }

    emit shopChanged();
}

QString ShopEditor::name() const
{
    if (!m_currentProject || !m_currentProject->currentCategory() || !m_currentProject->currentCategory()->currentShop()) return "";

    return m_currentProject->currentCategory()->currentShop()->name();
}

void ShopEditor::setName(QString name)
{
    if (m_currentProject && m_currentProject->currentCategory() && m_currentProject->currentCategory()->currentShop()) m_currentProject->currentCategory()->currentShop()->setName(name);

    madeChanges();
    emit categoryChanged();
    emit shopChanged();
}

QString ShopEditor::owner() const
{
    if (!m_currentProject || !m_currentProject->currentCategory() || !m_currentProject->currentCategory()->currentShop()) return "";

    return m_currentProject->currentCategory()->currentShop()->owner();
}

void ShopEditor::setOwner(QString owner)
{
    if (m_currentProject && m_currentProject->currentCategory() && m_currentProject->currentCategory()->currentShop()) m_currentProject->currentCategory()->currentShop()->setOwner(owner);

    madeChanges();
    emit shopChanged();
}

QString ShopEditor::description() const
{
    if (!m_currentProject || !m_currentProject->currentCategory() || !m_currentProject->currentCategory()->currentShop()) return "";

    return m_currentProject->currentCategory()->currentShop()->description();
}

void ShopEditor::setDescription(QString description)
{
    if (m_currentProject && m_currentProject->currentCategory() && m_currentProject->currentCategory()->currentShop()) m_currentProject->currentCategory()->currentShop()->setDescription(description);

    madeChanges();
    emit shopChanged();
}

void ShopEditor::onShopChanged()
{
    if (!m_currentProject || !m_currentProject->currentCategory() || !m_currentProject->currentCategory()->currentShop())
    {
        itemModel->clear();
        return;
    }

    itemModel->setElements(m_currentProject->currentCategory()->currentShop()->items());
}

void ShopEditor::moveShop(int positions)
{
    qDebug() << "ShopEditor: Moving shop by" << positions << "position(s) ...";

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

void ShopEditor::addItem(int index)
{
    if (!m_currentProject || !m_currentProject->currentCategory() || !m_currentProject->currentCategory()->currentShop() || !m_currentItemGroup) return;

    qDebug() << "ShopEditor: Adding item at index" << index << "...";

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

void ShopEditor::save()
{
    // Save to disc
    for (auto p : m_projects)
    {
        fileManager->getShopFileManager()->saveProject(p);
    }

    // Copy all projects and send them to ShopTool
    QList<ShopProject *> copies;

    for (auto p : m_projects)
    {
        ShopProject *copy = new ShopProject(p);
        copies.append(copy);
    }

    emit projectsSaved(copies);

    // Notify ShopEditor UI
    m_isSaved = true;
    emit isSavedChanged();
    emit showInfoBar(tr("Saved!"));
}

void ShopEditor::madeChanges()
{
    if (!m_currentProject) return;

    m_isSaved = false;
    emit isSavedChanged();
}

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

void ShopEditor::createProject(QString name)
{
    if (name.isEmpty()) return;

    auto p = new ShopProject(name, {});

    m_projects.append(p);
    emit projectListChanged();

    if (m_projects.size() == 1) setCurrentProject(0);

    madeChanges();
}

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

QStringList ShopEditor::itemGroups() const
{
    QStringList names;

    for (auto g : m_itemGroups)
    {
        if (g) names.append(g->name());
    }

    return names;
}

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
