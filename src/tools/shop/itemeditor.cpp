#include "itemeditor.h"
#include "logging.h"
#include "settings/settingsmanager.h"
#include "utils/utils.h"
#include "utils/fileutils.h"
#include "thirdparty/asyncfuture/asyncfuture.h"

#include <QJsonDocument>
#include <QQmlContext>

using namespace AsyncFuture;

ItemEditor::ItemEditor(QQmlApplicationEngine *engine, QObject *parent)
    : QObject(parent), a_isSaved(true), qmlEngine(engine)
{
    qDebug() << "Loading Item Editor ...";

    itemModel = new ItemModel;
    qmlEngine->rootContext()->setContextProperty("itemEditorItemModel", itemModel);
}

void ItemEditor::findItems()
{
    qCDebug(gmShopsItemEditor()) << "Finding items ...";

    const auto path = FileUtils::fileInDir("CustomItems.items", SettingsManager::getPath("shops"));
    observe(Files::File::getDataAsync(path)).subscribe([this](Files::FileDataResult *result) {
        if (!result) return;

        qCDebug(gmShopsItemEditor()) << "Received custom items.";

        m_itemGroup = new ItemGroup(tr("Custom"), QJsonDocument::fromJson(result->data()).object());
        updateCategories();
        updateItemModel();

        result->deleteLater();
    });
}

/**
 * @brief Add an item category
 * @param name Name of the category
 */
void ItemEditor::addCategory(const QString &name)
{
    qCDebug(gmShopsItemEditor()) << "Adding category" << name << "...";

    if (!categories().contains(name))
    {
        a_categories.append(name);
        emit categoriesChanged(categories());
        madeChanges();
    }
}

/**
 * @brief Get all categories from items
 */
void ItemEditor::updateCategories()
{
    a_categories.clear();

    if (m_itemGroup)
    {
        for (auto i : m_itemGroup->items())
        {
            if (i && !a_categories.contains(i->category()))
            {
                a_categories.append(i->category());
            }
        }
    }

    emit categoriesChanged(categories());
}

/**
 * @brief Add an item
 * @param name Name (Must not be empty)
 * @param price Price
 * @param category Category (Must not be empty)
 * @param description Description
 */
void ItemEditor::addItem(const QString &name, const QString &price, const QString &category, QString description)
{
    if (name.isEmpty() || category.isEmpty()) return;

    auto *item = new Item(name, price, description.replace("\n", " "), category);
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
    else m_itemGroup->items(items);

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

    if (Utils::isInBounds(m_itemGroup->items(), index))
    {
        auto items = m_itemGroup->items();
        auto item  = items.takeAt(index);
        m_itemGroup->items(items);
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
    if (isSaved()) return;

    // Save to disk
    const auto savePath = FileUtils::fileInDir("CustomItems.items", SettingsManager::getPath("shops"));
    const auto data     = QJsonDocument(m_itemGroup->toJson()).toJson();

    observe(Files::File::saveAsync(savePath, data)
    ).subscribe([this]() {
        // Notify editor
        isSaved(true);
        emit showInfoBar(tr("Saved!"));
    }, [this]() {
        emit showInfoBar(tr("Error: Could not save items!"));
    });

    // Copy all items and send them to the shop editor
    auto copy = new ItemGroup(m_itemGroup);
    emit itemsSaved(copy);
}

/**
 * @brief Notify UI that changes were made
 */
void ItemEditor::madeChanges()
{
    isSaved(false);
}
