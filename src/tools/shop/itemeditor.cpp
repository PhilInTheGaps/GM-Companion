#include "itemeditor.h"

#include <QDebug>
#include <QQmlContext>

ItemEditor::ItemEditor(FileManager *fManager, QQmlApplicationEngine *engine, QObject *parent)
    : QObject(parent), fileManager(fManager), qmlEngine(engine)
{
    qDebug() << "Loading Item Editor ...";

    itemModel = new ItemModel;
    qmlEngine->rootContext()->setContextProperty("itemEditorItemModel", itemModel);

    connect(fManager->getShopFileManager(), &ShopFileManager::receivedEditorItems, this, &ItemEditor::receivedItems);

    fManager->getShopFileManager()->findItems(fManager->getModeInt(), true);
}

/**
 * @brief ShopFileManager found items
 * @param group ItemGroup with custom items
 */
void ItemEditor::receivedItems(ItemGroup *group)
{
    qDebug() << "ItemEditor: Received items!";
    m_itemGroup = group;
    updateCategories();
    updateItemModel();
}

/**
 * @brief Add an item category
 * @param name Name of the category
 */
void ItemEditor::addCategory(QString name)
{
    qDebug() << "ItemEditor: Adding category" << name << "...";

    if (!m_categories.contains(name))
    {
        m_categories.append(name);
        emit categoriesChanged();
        madeChanges();
    }
}

/**
 * @brief Get all categories from items
 */
void ItemEditor::updateCategories()
{
    m_categories.clear();

    if (m_itemGroup)
    {
        for (auto i : m_itemGroup->items())
        {
            if (i && !m_categories.contains(i->category()))
            {
                m_categories.append(i->category());
            }
        }
    }

    emit categoriesChanged();
}

/**
 * @brief Add an item
 * @param name Name (Must not be empty)
 * @param price Price
 * @param category Category (Must not be empty)
 * @param description Description
 */
void ItemEditor::addItem(QString name, QString price, QString category, QString description)
{
    if (name.isEmpty() || category.isEmpty()) return;

    Item *item = new Item(name, price, description.replace("\n", " "), category);
    QList<Item *> items;
    int insert = -1;

    if (m_itemGroup)
    {
        items = m_itemGroup->items();

        // Find insertion index
        for (int i = 0; i < items.size(); i++)
        {
            if (items[i] && (items[i]->category() == category)) insert = i;
        }
    }

    if (insert < 0)
    {
        items.append(item);
    }
    else
    {
        items.insert(insert + 1, item);
    }

    if (!m_itemGroup) m_itemGroup = new ItemGroup(tr("Custom"), items);
    else m_itemGroup->setItems(items);

    itemModel->setElements(items);
    madeChanges();
}

/**
 * @brief Add items to item model
 */
void ItemEditor::updateItemModel()
{
    itemModel->clear();

    if (m_itemGroup)
    {
        itemModel->setElements(m_itemGroup->items());
    }
}

/**
 * @brief Remove an item
 * @param index Index of the item
 */
void ItemEditor::deleteItem(int index)
{
    if (!m_itemGroup) return;

    if ((index > -1) && (index < m_itemGroup->items().size()))
    {
        auto items = m_itemGroup->items();
        auto item  = items.takeAt(index);
        m_itemGroup->setItems(items);
        itemModel->setElements(items);

        item->deleteLater();
        madeChanges();
    }
}

/**
 * @brief Save the custom items to disc and send them to the shop editor
 */
void ItemEditor::save()
{
    // Save only when changes were made
    if (m_isSaved) return;

    // Save to disk
    fileManager->getShopFileManager()->saveItems(m_itemGroup);

    // Copy all items and send them to the shop editor
    ItemGroup *copy = new ItemGroup(m_itemGroup);
    emit itemsSaved(copy);

    // Notify editor
    m_isSaved = true;
    emit isSavedChanged();
    emit showInfoBar(tr("Saved!"));
}

/**
 * @brief Notify UI that changes were made
 */
void ItemEditor::madeChanges()
{
    m_isSaved = false;
    emit isSavedChanged();
}
