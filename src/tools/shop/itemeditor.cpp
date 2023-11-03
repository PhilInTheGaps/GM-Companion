#include "itemeditor.h"
#include "results/filecheckresult.h"
#include "results/filedataresult.h"
#include "settings/settingsmanager.h"
#include "utils/fileutils.h"
#include "utils/utils.h"
#include <QJsonDocument>
#include <QLoggingCategory>
#include <QQmlContext>
#include <filesystem/file.h>

using namespace Qt::Literals::StringLiterals;

Q_LOGGING_CATEGORY(gmShopsItemEditor, "gm.shops.items.editor")

auto ItemEditor::itemModel() -> ItemModel *
{
    return &m_itemModel;
}

/**
 * @brief Add an item category
 * @param name Name of the category
 */
auto ItemEditor::addCategory(const QString &name) -> bool
{
    qCDebug(gmShopsItemEditor()) << "Adding category" << name << "...";

    if (categories().contains(name)) return false;

    a_categories.append(name);
    emit categoriesChanged(categories());
    madeChanges();
    return true;
}

/**
 * @brief Get all categories from items
 */
void ItemEditor::updateCategories(const ItemGroup &group)
{
    a_categories.clear();

    foreach (const auto *item, group.items())
    {
        if (item && !a_categories.contains(item->category()))
        {
            a_categories.append(item->category());
        }
    }

    emit categoriesChanged(categories());
}

void ItemEditor::initItemModel(const ItemGroup &group)
{
    m_itemModel.replaceAll(Utils::toQObjectList(group.items()));
}

auto ItemEditor::addItem(const QString &name, const QString &price, const QString &category, QString description)
    -> bool
{
    if (name.isEmpty() || category.isEmpty()) return false;

    auto *item = new Item(name, price, description.replace("\n"_L1, " "_L1), category, this);
    const auto insert = Item::findLastIndexWithCategory(category, Utils::toList<Item *>(m_itemModel.getAll())) + 1;

    if (insert <= 0)
    {
        m_itemModel.append(item);
    }
    else
    {
        m_itemModel.insert(insert, item);
    }

    madeChanges();
    return true;
}

/**
 * @brief Remove an item
 * @param index Index of the item
 */
auto ItemEditor::deleteItem(int index) -> bool
{
    const bool success = m_itemModel.removeRow(index);

    if (success) madeChanges();

    return success;
}

/**
 * @brief Save the custom items and send them to the shop editor
 */
void ItemEditor::save()
{
    // Save only when changes were made
    if (isSaved()) return;

    // Save to disk
    const auto savePath = FileUtils::fileInDir(u"CustomItems.items"_s, SettingsManager::getPath(u"shops"_s));

    const auto group = ItemGroup(defaultGroupName(), Utils::toList<Item *>(m_itemModel.getAll()), nullptr);
    const auto data = QJsonDocument(group.toJson()).toJson();

    Files::File::saveAsync(savePath, data)
        .then([this](Files::FileResult &&) {
            // Notify editor
            isSaved(true);
            emit showInfoBar(tr("Saved!"));
        })
        .onCanceled([this]() { emit showInfoBar(tr("Error: Could not save items!")); });

    // Copy all items and send them to the shop editor
    auto *copy = new ItemGroup(group, this);
    emit itemsSaved(copy);
}

void ItemEditor::loadData()
{
    if (isDataLoaded()) return;

    isLoading(true);
    m_itemModel.clear();

    qCDebug(gmShopsItemEditor()) << "Finding items ...";

    const auto path = FileUtils::fileInDir(u"CustomItems.items"_s, SettingsManager::getPath(u"shops"_s));

    Files::File::checkAsync(path)
        .then([this](Files::FileCheckResult &&result) { onFileCheckReceived(std::move(result)); })
        .onCanceled([this]() { isLoading(false); });
}

/**
 * @brief Notify UI that changes were made
 */
void ItemEditor::madeChanges()
{
    isSaved(false);
}

auto ItemEditor::defaultGroupName() -> QString
{
    return tr("Custom");
}

void ItemEditor::onFileCheckReceived(Files::FileCheckResult &&result)
{
    const auto &path = result.path();

    if (!result.success() || !result.exists())
    {
        isLoading(false);
        return;
    }

    Files::File::getDataAsync(path)
        .then([this](Files::FileDataResult &&result) { onDataReceived(std::move(result)); })
        .onCanceled([this]() { isLoading(false); });
}

void ItemEditor::onDataReceived(Files::FileDataResult &&result)
{
    if (!result.success())
    {
        isLoading(false);
        return;
    }

    qCDebug(gmShopsItemEditor()) << "Received custom items.";

    ItemGroup const group(defaultGroupName(), QJsonDocument::fromJson(result.data()).object(), nullptr);

    initItemModel(group);
    updateCategories(group);

    setIsDataLoaded(true);
    isLoading(false);
}
